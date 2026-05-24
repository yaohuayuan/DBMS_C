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
#include "IndexManager.h"
#include "Layout.h"
#include "LogManager.h"
#include "MetadataManager.h"
#include "Page.h"
#include "Schema.h"
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

#define TEST_DB_DIR_PREFIX "metadata_basic_test_db"
#define TEST_LOG_FILE "metadata_basic.log"
#define TEST_BLOCK_SIZE 400
#define TEST_BUFFER_COUNT 12

typedef struct MetadataTestContext {
    char dbDirName[128];
    CString *dbDir;
    CString *logFile;
    FileManager *fileManager;
    LogManager *logManager;
    BufferManager *bufferManager;
} MetadataTestContext;

static int g_metadata_test_counter = 0;

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
    cleanup_db_file(directory, "tblcat.tbl");
    cleanup_db_file(directory, "fldcat.tbl");
    cleanup_db_file(directory, "viewcat.tbl");
    cleanup_db_file(directory, "idxcat.tbl");
    cleanup_db_file(directory, "student_meta.tbl");
    cleanup_db_file(directory, "student_idx_meta.tbl");
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

static int setup_metadata_context(void **state) {
    MetadataTestContext *ctx = calloc(1, sizeof(MetadataTestContext));
    assert_non_null(ctx);

    snprintf(ctx->dbDirName, sizeof(ctx->dbDirName), "%s_%d_%d",
             TEST_DB_DIR_PREFIX, GETPID(), g_metadata_test_counter++);
    cleanup_test_db(ctx->dbDirName);

    ctx->dbDir = CStringCreateFromCStr(ctx->dbDirName);
    ctx->logFile = CStringCreateFromCStr(TEST_LOG_FILE);
    ctx->fileManager = FileManagerInit(ctx->dbDir, TEST_BLOCK_SIZE);
    ctx->logManager = LogManagerInit(ctx->fileManager, ctx->logFile);
    ctx->bufferManager = BufferManagerInit(ctx->fileManager, ctx->logManager, TEST_BUFFER_COUNT, NULL);

    assert_non_null(ctx->dbDir);
    assert_non_null(ctx->logFile);
    assert_non_null(ctx->fileManager);
    assert_non_null(ctx->logManager);
    assert_non_null(ctx->bufferManager);

    *state = ctx;
    return 0;
}

static int teardown_metadata_context(void **state) {
    MetadataTestContext *ctx = *state;
    if (!ctx) {
        return 0;
    }

    destroy_buffer_manager(ctx->bufferManager);
    destroy_log_manager(ctx->logManager);
    FileManagerDestroy(ctx->fileManager);
    CStringDestroy(ctx->logFile);
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

static Transaction *new_transaction(MetadataTestContext *ctx) {
    Transaction *tx = TransactionInit(ctx->fileManager, ctx->logManager, ctx->bufferManager);
    assert_non_null(tx);
    return tx;
}

static void test_metadata_manager_create_table_and_read_layout(void **state) {
    MetadataTestContext *ctx = *state;
    Transaction *tx = new_transaction(ctx);
    MetadataMgr *metadata = MetadataMgrInit(true, tx);
    CString *tableName = CStringCreateFromCStr("student_meta");
    CString *idField = NULL;
    CString *nameField = NULL;
    Schema *schema = new_student_schema(&idField, &nameField);

    assert_non_null(metadata);
    assert_non_null(metadata->tblMgr);
    assert_non_null(metadata->viewMgr);
    assert_non_null(metadata->statMgr);
    assert_non_null(metadata->idxMgr);

    MetadataMgrCreateTable(metadata, tableName, schema, tx);
    Layout *layout = MetadataMgrGetLayout(metadata, tableName, tx);

    assert_non_null(layout);
    assert_non_null(layout->schema);
    assert_true(SchemaHasField(layout->schema, idField));
    assert_true(SchemaHasField(layout->schema, nameField));
    assert_int_equal(SchemaType(layout->schema, idField), FILE_INFO_CODE_INTEGER);
    assert_int_equal(SchemaType(layout->schema, nameField), FILE_INFO_CODE_VARCHAR);
    assert_int_equal(SchemaLength(layout->schema, idField), 4);
    assert_int_equal(SchemaLength(layout->schema, nameField), 20);
    assert_int_equal(LayoutOffset(layout, idField), sizeof(int));
    assert_int_equal(LayoutOffset(layout, nameField), sizeof(int) + sizeof(int));
    assert_true(layout->SlotSize > 0);

    TransactionCommit(tx);
    assert_int_equal(tx->code, TX_TRANSACTION_COMMIT);

    CStringDestroy(idField);
    CStringDestroy(nameField);
    CStringDestroy(tableName);
    SchemaFree(schema);
}

static void test_metadata_manager_create_view_and_read_definition(void **state) {
    MetadataTestContext *ctx = *state;
    Transaction *tx = new_transaction(ctx);
    MetadataMgr *metadata = MetadataMgrInit(true, tx);
    CString *viewName = CStringCreateFromCStr("student_view");
    CString *viewDef = CStringCreateFromCStr("select id, name from student_meta");

    assert_non_null(metadata);
    MetadataMgrCreateView(metadata, viewName, viewDef, tx);
    CString *readBack = MetadataMgrGetViewDef(metadata, viewName, tx);

    assert_non_null(readBack);
    assert_string_equal(CStringGetPtr(readBack), CStringGetPtr(viewDef));

    TransactionCommit(tx);
    assert_int_equal(tx->code, TX_TRANSACTION_COMMIT);

    CStringDestroy(readBack);
    CStringDestroy(viewDef);
    CStringDestroy(viewName);
}

static void test_metadata_manager_create_index_and_read_index_info(void **state) {
    MetadataTestContext *ctx = *state;
    Transaction *tx = new_transaction(ctx);
    MetadataMgr *metadata = MetadataMgrInit(true, tx);
    CString *tableName = CStringCreateFromCStr("student_idx_meta");
    CString *indexName = CStringCreateFromCStr("idx_student_id");
    CString *idField = NULL;
    CString *nameField = NULL;
    Schema *schema = new_student_schema(&idField, &nameField);

    MetadataMgrCreateTable(metadata, tableName, schema, tx);
    MetadataMgrCreateIndex(metadata, indexName, tableName, idField, tx);
    map_IndexInfo_t *indexInfoMap = MetadataManagerGetIndexInfo(metadata, tableName, tx);

    assert_non_null(indexInfoMap);
    assert_int_equal(indexInfoMap->base.nnodes, 1);
    IndexInfo *indexInfo = map_get(indexInfoMap, CStringGetPtr(idField));
    assert_non_null(indexInfo);
    assert_string_equal(CStringGetPtr(indexInfo->idxname), CStringGetPtr(indexName));
    assert_string_equal(CStringGetPtr(indexInfo->fldname), CStringGetPtr(idField));
    assert_non_null(indexInfo->layout);
    assert_non_null(indexInfo->statInfo);
    assert_true(SchemaHasField(indexInfo->tblSchema, idField));

    TransactionCommit(tx);
    assert_int_equal(tx->code, TX_TRANSACTION_COMMIT);

    map_deinit(indexInfoMap);
    free(indexInfoMap);
    CStringDestroy(idField);
    CStringDestroy(nameField);
    CStringDestroy(indexName);
    CStringDestroy(tableName);
    SchemaFree(schema);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_metadata_manager_create_table_and_read_layout,
                                        setup_metadata_context,
                                        teardown_metadata_context),
        cmocka_unit_test_setup_teardown(test_metadata_manager_create_view_and_read_definition,
                                        setup_metadata_context,
                                        teardown_metadata_context),
        cmocka_unit_test_setup_teardown(test_metadata_manager_create_index_and_read_index_info,
                                        setup_metadata_context,
                                        teardown_metadata_context),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
