#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmocka.h>

#include "BlockID.h"
#include "BufferManager.h"
#include "CString.h"
#include "FileManager.h"
#include "LogManager.h"
#include "Page.h"
#include "Transaction.h"

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

#define TEST_DB_DIR_PREFIX "transaction_basic_test_db"
#define TEST_DATA_FILE "tx_basic.tbl"
#define TEST_LOG_FILE "tx_basic.log"
#define TEST_BLOCK_SIZE 400
#define TEST_BUFFER_COUNT 3

typedef struct TransactionTestContext {
    char dbDirName[128];
    CString *dbDir;
    CString *dataFile;
    CString *logFile;
    FileManager *fileManager;
    LogManager *logManager;
    BufferManager *bufferManager;
} TransactionTestContext;

static int g_tx_test_counter = 0;

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

static int setup_transaction_context(void **state) {
    TransactionTestContext *ctx = calloc(1, sizeof(TransactionTestContext));
    assert_non_null(ctx);

    snprintf(ctx->dbDirName, sizeof(ctx->dbDirName), "%s_%d_%d",
             TEST_DB_DIR_PREFIX, GETPID(), g_tx_test_counter++);
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

static int teardown_transaction_context(void **state) {
    TransactionTestContext *ctx = *state;
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

static BlockID *new_test_block(TransactionTestContext *ctx, int blockNumber) {
    return BlockIDInit(ctx->dataFile, blockNumber);
}

static void test_transaction_init_reports_environment(void **state) {
    TransactionTestContext *ctx = *state;

    Transaction *transaction = TransactionInit(ctx->fileManager, ctx->logManager, ctx->bufferManager);

    assert_non_null(transaction);
    assert_int_equal(transaction->code, TX_TRANSACTION_RUN);
    assert_int_equal(TransactionBlockSize(transaction), TEST_BLOCK_SIZE);
    assert_int_equal(TransactionAvailableBuffs(transaction), TEST_BUFFER_COUNT);
    assert_non_null(transaction->bufferList);
    assert_non_null(transaction->conCurrencyManager);
    assert_non_null(transaction->recoveryManager);
}

static void test_transaction_pin_and_unpin_updates_available_buffers(void **state) {
    TransactionTestContext *ctx = *state;
    Transaction *transaction = TransactionInit(ctx->fileManager, ctx->logManager, ctx->bufferManager);
    BlockID *block = new_test_block(ctx, 0);

    assert_int_equal(TransactionAvailableBuffs(transaction), TEST_BUFFER_COUNT);

    TransactionPin(transaction, block);
    assert_int_equal(TransactionAvailableBuffs(transaction), TEST_BUFFER_COUNT - 1);

    TransactionUnPin(transaction, block);
    assert_int_equal(TransactionAvailableBuffs(transaction), TEST_BUFFER_COUNT);

    BlockIDDestroy(block);
}

static void test_transaction_set_int_commit_persists_value(void **state) {
    TransactionTestContext *ctx = *state;
    Transaction *transaction = TransactionInit(ctx->fileManager, ctx->logManager, ctx->bufferManager);
    BlockID *block = new_test_block(ctx, 1);

    TransactionPin(transaction, block);
    TransactionSetInt(transaction, block, 32, 2026, false);
    assert_int_equal(TransactionGetInt(transaction, block, 32), 2026);

    TransactionCommit(transaction);
    assert_int_equal(transaction->code, TX_TRANSACTION_COMMIT);
    assert_int_equal(TransactionAvailableBuffs(transaction), TEST_BUFFER_COUNT);

    Page *readPage = PageInit(TEST_BLOCK_SIZE);
    FileManagerRead(ctx->fileManager, block, readPage);
    assert_int_equal(PageGetInt(readPage, 32), 2026);

    PageDestroy(readPage);
    BlockIDDestroy(block);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_transaction_init_reports_environment,
                                        setup_transaction_context,
                                        teardown_transaction_context),
        cmocka_unit_test_setup_teardown(test_transaction_pin_and_unpin_updates_available_buffers,
                                        setup_transaction_context,
                                        teardown_transaction_context),
        cmocka_unit_test_setup_teardown(test_transaction_set_int_commit_persists_value,
                                        setup_transaction_context,
                                        teardown_transaction_context),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
