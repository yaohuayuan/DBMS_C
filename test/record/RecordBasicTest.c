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
#include "Layout.h"
#include "LogManager.h"
#include "Page.h"
#include "RecordPage.h"
#include "RID.h"
#include "Scan.h"
#include "Schema.h"
#include "TableScan.h"
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

#define TEST_DB_DIR_PREFIX "record_basic_test_db"
#define TEST_DATA_FILE "record_basic.tbl"
#define TEST_SCAN_TABLE "record_basic_scan"
#define TEST_SCAN_FILE "record_basic_scan.tbl"
#define TEST_LOG_FILE "record_basic.log"
#define TEST_BLOCK_SIZE 400
#define TEST_BUFFER_COUNT 6

typedef struct RecordTestContext {
    char dbDirName[128];
    CString *dbDir;
    CString *dataFile;
    CString *logFile;
    FileManager *fileManager;
    LogManager *logManager;
    BufferManager *bufferManager;
} RecordTestContext;

static int g_record_test_counter = 0;

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
    cleanup_db_file(directory, TEST_SCAN_FILE);
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

static int setup_record_context(void **state) {
    RecordTestContext *ctx = calloc(1, sizeof(RecordTestContext));
    assert_non_null(ctx);

    snprintf(ctx->dbDirName, sizeof(ctx->dbDirName), "%s_%d_%d",
             TEST_DB_DIR_PREFIX, GETPID(), g_record_test_counter++);
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

static int teardown_record_context(void **state) {
    RecordTestContext *ctx = *state;
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

static Schema *new_student_schema(CString **idField, CString **nameField) {
    Schema *schema = SchemaInit();
    assert_non_null(schema);

    *idField = CStringCreateFromCStr("id");
    *nameField = CStringCreateFromCStr("name");
    assert_non_null(*idField);
    assert_non_null(*nameField);

    SchemaAddIntField(schema, *idField);
    SchemaAddStringField(schema, *nameField, 20);
    return schema;
}

static void destroy_layout(Layout *layout) {
    if (!layout) {
        return;
    }
    if (layout->offsets) {
        map_deinit(layout->offsets);
        free(layout->offsets);
    }
    free(layout);
}

static void destroy_scan_wrapper(Scan *scan) {
    if (!scan) {
        return;
    }
    free(scan);
}

static void test_schema_adds_int_and_string_fields(void **state) {
    (void) state;
    CString *idField = NULL;
    CString *nameField = NULL;
    Schema *schema = new_student_schema(&idField, &nameField);

    assert_true(SchemaHasField(schema, idField));
    assert_true(SchemaHasField(schema, nameField));
    assert_int_equal(SchemaType(schema, idField), FILE_INFO_CODE_INTEGER);
    assert_int_equal(SchemaType(schema, nameField), FILE_INFO_CODE_VARCHAR);
    assert_int_equal(SchemaLength(schema, idField), 4);
    assert_int_equal(SchemaLength(schema, nameField), 20);

    CStringDestroy(idField);
    CStringDestroy(nameField);
    SchemaFree(schema);
}

static void test_layout_computes_offsets_and_slot_size(void **state) {
    (void) state;
    CString *idField = NULL;
    CString *nameField = NULL;
    Schema *schema = new_student_schema(&idField, &nameField);
    Layout *layout = LayoutInit(schema, NULL, 0);
    assert_non_null(layout);

    assert_int_equal(LayoutOffset(layout, idField), sizeof(int));
    assert_int_equal(LayoutOffset(layout, nameField), sizeof(int) + sizeof(int));
    assert_int_equal(layout->SlotSize, sizeof(int) + sizeof(int) + sizeof(int) + 20);

    destroy_layout(layout);
    CStringDestroy(idField);
    CStringDestroy(nameField);
    SchemaFree(schema);
}

static void test_rid_stores_identity(void **state) {
    (void) state;
    RID *rid = RIDInit(3, 7);
    RID *same = RIDInit(3, 7);
    RID *different = RIDInit(4, 7);
    char *ridString = RIDToString(rid);

    assert_non_null(rid);
    assert_non_null(same);
    assert_non_null(different);
    assert_int_equal(rid->BlockNum, 3);
    assert_int_equal(rid->Slot, 7);
    assert_true(RIDEqual(rid, same));
    assert_false(RIDEqual(rid, different));
    assert_non_null(ridString);
    assert_string_equal(ridString, "(3,7)");

    free(ridString);
    free(different);
    free(same);
    free(rid);
}

static void test_record_page_insert_set_and_read_fields(void **state) {
    RecordTestContext *ctx = *state;
    Transaction *transaction = TransactionInit(ctx->fileManager, ctx->logManager, ctx->bufferManager);
    BlockID *block = BlockIDInit(ctx->dataFile, 0);
    CString *idField = NULL;
    CString *nameField = NULL;
    Schema *schema = new_student_schema(&idField, &nameField);
    Layout *layout = LayoutInit(schema, NULL, 0);
    RecordPage *recordPage = RecordPageInit(transaction, block, layout);

    assert_non_null(transaction);
    assert_non_null(block);
    assert_non_null(layout);
    assert_non_null(recordPage);

    RecordPageFormat(recordPage);
    int slot = RecordPageInsertAfter(recordPage, -1);
    assert_true(slot >= 0);
    assert_int_equal(RecordPageNextAfter(recordPage, -1), slot);

    CString *nameValue = CStringCreateFromCStr("alice");
    RecordSetInt(recordPage, slot, idField, 101);
    RecordSetString(recordPage, slot, nameField, nameValue);

    assert_int_equal(RecordPageGetInt(recordPage, slot, idField), 101);
    assert_string_equal(RecordPageGetString(recordPage, slot, nameField), "alice");

    TransactionCommit(transaction);
    assert_int_equal(transaction->code, TX_TRANSACTION_COMMIT);

    CStringDestroy(nameValue);
    free(recordPage);
    destroy_layout(layout);
    CStringDestroy(idField);
    CStringDestroy(nameField);
    SchemaFree(schema);
    BlockIDDestroy(block);
}

static void test_table_scan_insert_and_read_record(void **state) {
    RecordTestContext *ctx = *state;
    Transaction *transaction = TransactionInit(ctx->fileManager, ctx->logManager, ctx->bufferManager);
    CString *tableName = CStringCreateFromCStr(TEST_SCAN_TABLE);
    CString *idField = NULL;
    CString *nameField = NULL;
    Schema *schema = new_student_schema(&idField, &nameField);
    Layout *layout = LayoutInit(schema, NULL, 0);
    TableScan *tableScan = TableScanInit(transaction, tableName, layout);
    Scan *scan = ScanInit(tableScan, SCAN_TABLE_CODE);

    assert_non_null(transaction);
    assert_non_null(tableName);
    assert_non_null(layout);
    assert_non_null(tableScan);
    assert_non_null(scan);

    TableScanInsert(scan);
    CString *nameValue = CStringCreateFromCStr("bob");
    TableScanSetInt(scan, idField, 202);
    TableScanSetString(scan, nameField, nameValue);

    RID *rid = TableScanGetRID(scan);
    assert_non_null(rid);
    assert_true(rid->BlockNum >= 0);
    assert_true(rid->Slot >= 0);

    TableScanBeforeFirst(scan);
    assert_true(TableScanNext(scan));
    assert_int_equal(TableScanGetInt(scan, idField), 202);
    assert_string_equal(TableScanGetString(scan, nameField), "bob");
    assert_false(TableScanNext(scan));

    TransactionCommit(transaction);
    assert_int_equal(transaction->code, TX_TRANSACTION_COMMIT);

    free(rid);
    CStringDestroy(nameValue);
    destroy_scan_wrapper(scan);
    free(tableScan);
    destroy_layout(layout);
    CStringDestroy(idField);
    CStringDestroy(nameField);
    CStringDestroy(tableName);
    SchemaFree(schema);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_schema_adds_int_and_string_fields),
        cmocka_unit_test(test_layout_computes_offsets_and_slot_size),
        cmocka_unit_test(test_rid_stores_identity),
        cmocka_unit_test_setup_teardown(test_record_page_insert_set_and_read_fields,
                                        setup_record_context,
                                        teardown_record_context),
        cmocka_unit_test_setup_teardown(test_table_scan_insert_and_read_record,
                                        setup_record_context,
                                        teardown_record_context),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
