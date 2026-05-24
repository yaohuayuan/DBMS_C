#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"

#include <stdio.h>
#include <stdlib.h>

#include "BasicQueryPlanner.h"
#include "BlockID.h"
#include "BufferManager.h"
#include "CString.h"
#include "FileManager.h"
#include "Layout.h"
#include "LogManager.h"
#include "MetadataManager.h"
#include "Page.h"
#include "Planner.h"
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

#define TEST_DB_DIR_PREFIX "plan_scan_basic_test_db"
#define TEST_LOG_FILE "plan_scan_basic.log"
#define TEST_TABLE_NAME "student_plan"
#define TEST_TABLE_FILE "student_plan.tbl"
#define TEST_BLOCK_SIZE 400
#define TEST_BUFFER_COUNT 10

typedef struct PlanScanTestContext {
    char dbDirName[128];
    CString *dbDir;
    CString *logFile;
    FileManager *fileManager;
    LogManager *logManager;
    BufferManager *bufferManager;
} PlanScanTestContext;

static int g_plan_scan_counter = 0;

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
    cleanup_db_file(directory, TEST_TABLE_FILE);
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

static int setup_plan_scan_context(void **state) {
    PlanScanTestContext *ctx = calloc(1, sizeof(PlanScanTestContext));
    assert_non_null(ctx);

    snprintf(ctx->dbDirName, sizeof(ctx->dbDirName), "%s_%d_%d",
             TEST_DB_DIR_PREFIX, GETPID(), g_plan_scan_counter++);
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

static int teardown_plan_scan_context(void **state) {
    PlanScanTestContext *ctx = *state;
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

static void insert_student(Scan *scan, CString *idField, CString *nameField, int id, const char *name) {
    CString *nameValue = CStringCreateFromCStr(name);
    assert_non_null(nameValue);

    TableScanInsert(scan);
    TableScanSetInt(scan, idField, id);
    TableScanSetString(scan, nameField, nameValue);

    CStringDestroy(nameValue);
}

static Planner *prepare_student_plan_database(PlanScanTestContext *ctx, Transaction **txOut) {
    Transaction *tx = TransactionInit(ctx->fileManager, ctx->logManager, ctx->bufferManager);
    assert_non_null(tx);

    MetadataMgr *metadata = MetadataMgrInit(true, tx);
    assert_non_null(metadata);

    CString *tableName = CStringCreateFromCStr(TEST_TABLE_NAME);
    CString *idField = NULL;
    CString *nameField = NULL;
    Schema *schema = new_student_schema(&idField, &nameField);
    MetadataMgrCreateTable(metadata, tableName, schema, tx);

    Layout *layout = MetadataMgrGetLayout(metadata, tableName, tx);
    assert_non_null(layout);
    TableScan *tableScan = TableScanInit(tx, tableName, layout);
    Scan *scan = ScanInit(tableScan, SCAN_TABLE_CODE);
    assert_non_null(scan);

    insert_student(scan, idField, nameField, 1, "wang");
    insert_student(scan, idField, nameField, 2, "li");
    TableScanClose(scan);
    free(scan);

    BasicQueryPlanner *queryPlanner = BasicQueryPlannerInit(metadata);
    Planner *planner = PlannerInit(queryPlanner, NULL);
    assert_non_null(planner);
    assert_non_null(planner->queryPlanner);

    CStringDestroy(idField);
    CStringDestroy(nameField);
    CStringDestroy(tableName);
    *txOut = tx;
    return planner;
}

static void assert_student_query_result(Scan *scan) {
    CString *idField = CStringCreateFromCStr("id");
    CString *nameField = CStringCreateFromCStr("name");
    CString *missingField = CStringCreateFromCStr("missing");

    assert_non_null(scan);
    assert_true(scan->hasField(scan, idField));
    assert_true(scan->hasField(scan, nameField));
    assert_false(scan->hasField(scan, missingField));

    assert_true(scan->next(scan));
    assert_int_equal(scan->getInt(scan, idField), 1);
    assert_string_equal(scan->getString(scan, nameField), "wang");
    assert_false(scan->next(scan));

    CStringDestroy(missingField);
    CStringDestroy(nameField);
    CStringDestroy(idField);
}

static void test_basic_query_planner_select_project_scan(void **state) {
    PlanScanTestContext *ctx = *state;
    Transaction *tx = NULL;
    Planner *planner = prepare_student_plan_database(ctx, &tx);
    CString *query = CStringCreateFromCStr("select id, name from student_plan where id = 1");

    Plan *plan = PlannerCreateQueryPlan(planner, query, tx);

    assert_non_null(plan);
    assert_int_equal(plan->code, PLAN_PROJECT_CODE);
    assert_non_null(plan->planUnion.projectPlan);
    assert_int_equal(plan->planUnion.projectPlan->p->code, PLAN_SELECT_CODE);
    assert_int_equal(plan->planUnion.projectPlan->p->planUnion.selectPlan->p->code, PLAN_TABLE_CODE);

    Scan *scan = plan->open(plan);
    assert_non_null(scan);
    assert_int_equal(scan->code, SCAN_PROJECT_CODE);
    assert_student_query_result(scan);
    scan->close(scan);

    TransactionCommit(tx);
    assert_int_equal(tx->code, TX_TRANSACTION_COMMIT);
    CStringDestroy(query);
}

static void test_plan_open_returns_scan_for_query_result(void **state) {
    PlanScanTestContext *ctx = *state;
    Transaction *tx = NULL;
    Planner *planner = prepare_student_plan_database(ctx, &tx);
    CString *query = CStringCreateFromCStr("select id, name from student_plan where id = 1");

    Plan *plan = PlannerCreateQueryPlan(planner, query, tx);
    Scan *scan = plan->open(plan);

    assert_non_null(plan);
    assert_non_null(scan);
    assert_int_equal(scan->code, SCAN_PROJECT_CODE);
    assert_student_query_result(scan);

    scan->close(scan);
    TransactionCommit(tx);
    assert_int_equal(tx->code, TX_TRANSACTION_COMMIT);
    CStringDestroy(query);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_basic_query_planner_select_project_scan,
                                        setup_plan_scan_context,
                                        teardown_plan_scan_context),
        cmocka_unit_test_setup_teardown(test_plan_open_returns_scan_for_query_result,
                                        setup_plan_scan_context,
                                        teardown_plan_scan_context),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
