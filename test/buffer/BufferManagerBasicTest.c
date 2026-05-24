#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmocka.h>

#include "BlockID.h"
#include "Buffer.h"
#include "BufferManager.h"
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

#define TEST_DB_DIR_PREFIX "buffer_manager_basic_test_db"
#define TEST_DATA_FILE "buffer_basic.tbl"
#define TEST_LOG_FILE "buffer_basic.log"
#define TEST_BLOCK_SIZE 400
#define TEST_BUFFER_COUNT 2

typedef struct BufferTestContext {
    char dbDirName[128];
    CString *dbDir;
    CString *dataFile;
    CString *logFile;
    FileManager *fileManager;
    LogManager *logManager;
    BufferManager *bufferManager;
} BufferTestContext;

static int g_buffer_test_counter = 0;

static void cleanup_db_file(const char *directory, const char *filename) {
    char path[256];
    snprintf(path, sizeof(path), "%s/%s", directory, filename);
    remove(path);
#ifdef _WIN32
    snprintf(path, sizeof(path), "%s\\%s", directory, filename);
    remove(path);
    DeleteFileA(path);
#endif
}

static void cleanup_test_db(const char *directory) {
    cleanup_db_file(directory, TEST_DATA_FILE);
    cleanup_db_file(directory, TEST_LOG_FILE);
#ifdef _WIN32
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

static void destroy_buffer_manager(BufferManager *bufferManager) {
    if (!bufferManager) {
        return;
    }
    if (bufferManager->policy) {
        bufferManager->policy->destroy(bufferManager->policy->impl);
        free(bufferManager->policy);
    }
    if (bufferManager->bufferPool) {
        for (int i = 0; i < bufferManager->bufferSize; i++) {
            Buffer *buffer = *(Buffer **)CVectorAt(bufferManager->bufferPool, i);
            if (buffer) {
                if (buffer->page) {
                    PageDestroy(buffer->page);
                }
                if (buffer->blockId) {
                    BlockIDDestroy(buffer->blockId);
                }
                free(buffer);
            }
        }
        CVectorDestroy(bufferManager->bufferPool);
    }
    free(bufferManager);
}

static int setup_buffer_context(void **state) {
    BufferTestContext *ctx = calloc(1, sizeof(BufferTestContext));
    assert_non_null(ctx);

    snprintf(ctx->dbDirName, sizeof(ctx->dbDirName), "%s_%d_%d",
             TEST_DB_DIR_PREFIX, GETPID(), g_buffer_test_counter++);
    cleanup_test_db(ctx->dbDirName);

    ctx->dbDir = CStringCreateFromCStr(ctx->dbDirName);
    ctx->dataFile = CStringCreateFromCStr(TEST_DATA_FILE);
    ctx->logFile = CStringCreateFromCStr(TEST_LOG_FILE);
    ctx->fileManager = FileManagerInit(ctx->dbDir, TEST_BLOCK_SIZE);
    ctx->logManager = LogManagerInit(ctx->fileManager, ctx->logFile);
    ctx->bufferManager = BufferManagerInit(ctx->fileManager, ctx->logManager, TEST_BUFFER_COUNT, NULL);

    assert_non_null(ctx->dbDir);
    assert_non_null(ctx->dataFile);
    assert_non_null(ctx->logFile);
    assert_non_null(ctx->fileManager);
    assert_non_null(ctx->logManager);
    assert_non_null(ctx->bufferManager);

    *state = ctx;
    return 0;
}

static int teardown_buffer_context(void **state) {
    BufferTestContext *ctx = *state;
    if (!ctx) {
        return 0;
    }

    destroy_buffer_manager(ctx->bufferManager);
    destroy_log_manager(ctx->logManager);
    FileManagerDestroy(ctx->fileManager);
    CStringDestroy(ctx->logFile);
    CStringDestroy(ctx->dataFile);
    CStringDestroy(ctx->dbDir);
    cleanup_test_db(ctx->dbDirName);
    free(ctx);
    return 0;
}

static BlockID *new_test_block(BufferTestContext *ctx, int blockNumber) {
    return BlockIDInit(ctx->dataFile, blockNumber);
}

static void test_buffer_manager_pin_and_unpin_updates_available(void **state) {
    BufferTestContext *ctx = *state;
    BufferManager *bufferManager = ctx->bufferManager;
    BlockID *block = new_test_block(ctx, 0);

    assert_int_equal(bufferManager->numAvailable, TEST_BUFFER_COUNT);

    Buffer *buffer = BufferManagerPin(bufferManager, block);
    assert_non_null(buffer);
    assert_true(BufferIsPinned(buffer));
    assert_int_equal(buffer->pins, 1);
    assert_int_equal(bufferManager->numAvailable, TEST_BUFFER_COUNT - 1);
    assert_true(BlockIDEqual(buffer->blockId, block));

    BufferManagerUnpin(bufferManager, buffer);
    assert_false(BufferIsPinned(buffer));
    assert_int_equal(bufferManager->numAvailable, TEST_BUFFER_COUNT);

    BlockIDDestroy(block);
}

static void test_buffer_manager_repin_existing_block(void **state) {
    BufferTestContext *ctx = *state;
    BufferManager *bufferManager = ctx->bufferManager;
    BlockID *block = new_test_block(ctx, 1);

    Buffer *first = BufferManagerPin(bufferManager, block);
    Buffer *second = BufferManagerPin(bufferManager, block);

    assert_non_null(first);
    assert_ptr_equal(first, second);
    assert_int_equal(first->pins, 2);
    assert_int_equal(bufferManager->numAvailable, TEST_BUFFER_COUNT - 1);

    BufferManagerUnpin(bufferManager, first);
    assert_true(BufferIsPinned(first));
    assert_int_equal(bufferManager->numAvailable, TEST_BUFFER_COUNT - 1);

    BufferManagerUnpin(bufferManager, first);
    assert_false(BufferIsPinned(first));
    assert_int_equal(bufferManager->numAvailable, TEST_BUFFER_COUNT);

    BlockIDDestroy(block);
}

static void test_buffer_manager_dirty_flush_writes_page(void **state) {
    BufferTestContext *ctx = *state;
    BufferManager *bufferManager = ctx->bufferManager;
    BlockID *block = new_test_block(ctx, 2);

    Buffer *buffer = BufferManagerPin(bufferManager, block);
    assert_non_null(buffer);

    PageSetInt(buffer->page, 24, 2026);
    BufferSetModified(buffer, 77, -1);
    BufferManagerUnpin(bufferManager, buffer);

    BufferManagerFlushAll(bufferManager, 77);
    assert_int_equal(buffer->txNum, -1);

    Page *readPage = PageInit(TEST_BLOCK_SIZE);
    FileManagerRead(ctx->fileManager, block, readPage);
    assert_int_equal(PageGetInt(readPage, 24), 2026);

    PageDestroy(readPage);
    BlockIDDestroy(block);
}

static void test_buffer_manager_can_reuse_unpinned_frame(void **state) {
    BufferTestContext *ctx = *state;
    BufferManager *bufferManager = ctx->bufferManager;
    BlockID *firstBlock = new_test_block(ctx, 3);
    BlockID *secondBlock = new_test_block(ctx, 4);
    BlockID *thirdBlock = new_test_block(ctx, 5);

    Buffer *first = BufferManagerPin(bufferManager, firstBlock);
    Buffer *second = BufferManagerPin(bufferManager, secondBlock);
    assert_non_null(first);
    assert_non_null(second);
    assert_int_equal(bufferManager->numAvailable, 0);

    BufferManagerUnpin(bufferManager, first);
    assert_int_equal(bufferManager->numAvailable, 1);

    Buffer *third = BufferManagerPin(bufferManager, thirdBlock);
    assert_non_null(third);
    assert_int_equal(bufferManager->numAvailable, 0);
    assert_true(BlockIDEqual(third->blockId, thirdBlock));

    BufferManagerUnpin(bufferManager, third);
    BufferManagerUnpin(bufferManager, second);
    assert_int_equal(bufferManager->numAvailable, TEST_BUFFER_COUNT);

    BlockIDDestroy(thirdBlock);
    BlockIDDestroy(secondBlock);
    BlockIDDestroy(firstBlock);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_buffer_manager_pin_and_unpin_updates_available,
                                        setup_buffer_context,
                                        teardown_buffer_context),
        cmocka_unit_test_setup_teardown(test_buffer_manager_repin_existing_block,
                                        setup_buffer_context,
                                        teardown_buffer_context),
        cmocka_unit_test_setup_teardown(test_buffer_manager_dirty_flush_writes_page,
                                        setup_buffer_context,
                                        teardown_buffer_context),
        cmocka_unit_test_setup_teardown(test_buffer_manager_can_reuse_unpinned_frame,
                                        setup_buffer_context,
                                        teardown_buffer_context),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
