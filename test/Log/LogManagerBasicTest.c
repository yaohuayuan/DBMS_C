#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

#include "BlockID.h"
#include "ByteBuffer.h"
#include "CString.h"
#include "FileManager.h"
#include "LogManager.h"
#include "Page.h"

#ifdef _WIN32
#include <direct.h>
#include <process.h>
#include <windows.h>
#define RMDIR(path) _rmdir(path)
#define GETPID() _getpid()
#else
#include <unistd.h>
#define RMDIR(path) rmdir(path)
#define GETPID() getpid()
#endif

#define TEST_DB_DIR_PREFIX "log_manager_basic_test_db"
#define TEST_LOG_FILE "test_logfile.log"
#define TEST_BLOCK_SIZE 400

typedef struct LogTestContext {
    char dbDirName[128];
    CString *dbDir;
    CString *logFile;
    FileManager *fileManager;
    LogManager *logManager;
} LogTestContext;

static int g_log_test_counter = 0;

static void cleanup_db_file(const char *directory, const char *filename) {
    char path[256];
    snprintf(path, sizeof(path), "%s/%s", directory, filename);
    remove(path);
#ifdef _WIN32
    snprintf(path, sizeof(path), "%s\\%s", directory, filename);
    remove(path);
    DeleteFileA(path);
    RemoveDirectoryA(directory);
#endif
    RMDIR(directory);
}

static void destroy_log_manager(LogManager *logManager) {
    if (!logManager) {
        return;
    }
    if (logManager->logPage) {
        PageDestroy(logManager->logPage);
    }
    if (logManager->currentBlockId) {
        BlockIDDestroy(logManager->currentBlockId);
    }
    if (logManager->logFile) {
        CStringDestroy(logManager->logFile);
    }
    free(logManager);
}

static void destroy_log_iterator(LogIterator *iterator) {
    if (!iterator) {
        return;
    }
    if (iterator->page) {
        PageDestroy(iterator->page);
    }
    if (iterator->blockId) {
        BlockIDDestroy(iterator->blockId);
    }
    free(iterator);
}

static int setup_log_context(void **state) {
    LogTestContext *ctx = calloc(1, sizeof(LogTestContext));
    assert_non_null(ctx);

    snprintf(ctx->dbDirName, sizeof(ctx->dbDirName), "%s_%d_%d",
             TEST_DB_DIR_PREFIX, GETPID(), g_log_test_counter++);
    cleanup_db_file(ctx->dbDirName, TEST_LOG_FILE);

    ctx->dbDir = CStringCreateFromCStr(ctx->dbDirName);
    ctx->logFile = CStringCreateFromCStr(TEST_LOG_FILE);
    ctx->fileManager = FileManagerInit(ctx->dbDir, TEST_BLOCK_SIZE);
    ctx->logManager = LogManagerInit(ctx->fileManager, ctx->logFile);

    assert_non_null(ctx->dbDir);
    assert_non_null(ctx->logFile);
    assert_non_null(ctx->fileManager);
    assert_non_null(ctx->logManager);

    *state = ctx;
    return 0;
}

static int teardown_log_context(void **state) {
    LogTestContext *ctx = *state;
    if (!ctx) {
        return 0;
    }

    destroy_log_manager(ctx->logManager);
    FileManagerDestroy(ctx->fileManager);
    CStringDestroy(ctx->logFile);
    CStringDestroy(ctx->dbDir);
    cleanup_db_file(ctx->dbDirName, TEST_LOG_FILE);
    free(ctx);
    return 0;
}

static void append_text_record(LogManager *logManager, const char *text, int expectedLsn) {
    int lsn = LogManagerAppend(logManager, (const uint8_t *)text, (uint32_t)strlen(text) + 1);
    assert_int_equal(lsn, expectedLsn);
}

static void assert_next_text(LogIterator *iterator, const char *expected) {
    assert_true(LogIteratorHasNext(iterator));

    ByteBuffer *record = LogIteratorNext(iterator);
    assert_non_null(record);
    assert_int_equal(record->size, strlen(expected) + 1);
    assert_string_equal((const char *)record->data, expected);

    bufferFree(record);
}

static void test_log_manager_append_and_flush(void **state) {
    LogTestContext *ctx = *state;

    append_text_record(ctx->logManager, "hello-log", 1);
    assert_int_equal(ctx->logManager->latestLSN, 1);
    assert_int_equal(ctx->logManager->LastSavedLSN, 0);

    LogManagerFlush(ctx->logManager);

    assert_int_equal(ctx->logManager->LastSavedLSN, 1);
    assert_int_equal(FileManagerLength(ctx->fileManager, ctx->logFile), 1);
}

static void test_log_manager_iterate_records_reverse_order(void **state) {
    LogTestContext *ctx = *state;

    append_text_record(ctx->logManager, "first", 1);
    append_text_record(ctx->logManager, "second", 2);
    append_text_record(ctx->logManager, "third", 3);
    LogManagerFlush(ctx->logManager);

    LogIterator *iterator = LogManager2LogManager(ctx->logManager);
    assert_next_text(iterator, "third");
    assert_next_text(iterator, "second");
    assert_next_text(iterator, "first");
    assert_false(LogIteratorHasNext(iterator));

    destroy_log_iterator(iterator);
}

static void test_log_manager_persists_after_reopen(void **state) {
    LogTestContext *ctx = *state;

    append_text_record(ctx->logManager, "persisted-first", 1);
    append_text_record(ctx->logManager, "persisted-second", 2);
    LogManagerFlush(ctx->logManager);

    destroy_log_manager(ctx->logManager);
    FileManagerDestroy(ctx->fileManager);

    ctx->fileManager = FileManagerInit(ctx->dbDir, TEST_BLOCK_SIZE);
    ctx->logManager = LogManagerInit(ctx->fileManager, ctx->logFile);
    assert_non_null(ctx->fileManager);
    assert_non_null(ctx->logManager);

    LogIterator *iterator = LogManager2LogManager(ctx->logManager);
    assert_next_text(iterator, "persisted-second");
    assert_next_text(iterator, "persisted-first");
    assert_false(LogIteratorHasNext(iterator));

    destroy_log_iterator(iterator);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_log_manager_append_and_flush,
                                        setup_log_context,
                                        teardown_log_context),
        cmocka_unit_test_setup_teardown(test_log_manager_iterate_records_reverse_order,
                                        setup_log_context,
                                        teardown_log_context),
        cmocka_unit_test_setup_teardown(test_log_manager_persists_after_reopen,
                                        setup_log_context,
                                        teardown_log_context),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
