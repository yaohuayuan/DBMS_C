#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "BasicUpdatePlanner.h"
#include "BlockID.h"
#include "BufferManager.h"
#include "CString.h"
#include "FileManager.h"
#include "IndexInfo.h"
#include "Layout.h"
#include "LogManager.h"
#include "MetadataManager.h"
#include "Page.h"
#include "Planner.h"
#include "Scan.h"
#include "Schema.h"
#include "TableScan.h"
#include "Transaction.h"
#include "map.h"

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

#define TEST_DB_DIR_PREFIX "update_basic_test_db"
#define TEST_LOG_FILE "update_basic.log"
#define TEST_TABLE_NAME "update_student"
#define TEST_TABLE_FILE "update_student.tbl"
#define TEST_BLOCK_SIZE 400
#define TEST_BUFFER_COUNT 12

typedef struct UpdateTestContext {
    char dbDirName[128];
    CString *dbDir;
    CString *logFile;
    FileManager *fileManager;
    LogManager *logManager;
    BufferManager *bufferManager;
} UpdateTestContext;

typedef struct StudentRow {
    bool found;
    int id;
    char name[64];
} StudentRow;

static int g_update_test_counter = 0;

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

static int setup_update_context(void **state) {
    UpdateTestContext *ctx = calloc(1, sizeof(UpdateTestContext));
    assert_non_null(ctx);

    snprintf(ctx->dbDirName, sizeof(ctx->dbDirName), "%s_%d_%d",
             TEST_DB_DIR_PREFIX, GETPID(), g_update_test_counter++);
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

static int teardown_update_context(void **state) {
    UpdateTestContext *ctx = *state;
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

static Transaction *new_transaction(UpdateTestContext *ctx) {
    Transaction *tx = TransactionInit(ctx->fileManager, ctx->logManager, ctx->bufferManager);
    assert_non_null(tx);
    return tx;
}

static Planner *new_update_planner(MetadataMgr *metadata) {
    BasicUpdatePlanner *updatePlanner = BasicUpdatePlannerInit(metadata);
    Planner *planner = PlannerInit(NULL, updatePlanner);

    assert_non_null(updatePlanner);
    assert_non_null(planner);
    assert_non_null(planner->updatePlanner);
    return planner;
}

static int execute_update(Planner *planner, Transaction *tx, const char *sql) {
    CString *command = CStringCreateFromCStr(sql);
    assert_non_null(command);

    int result = PlannerExecuteUpdate(planner, command, tx);

    CStringDestroy(command);
    return result;
}

static void create_update_student_table(Planner *planner, Transaction *tx) {
    int result = execute_update(planner, tx,
                                "create table update_student(id int, name varchar(20))");
    assert_int_equal(result, 0);
}

static void insert_update_student_rows(Planner *planner, Transaction *tx) {
    assert_int_equal(execute_update(planner, tx,
                                    "insert into update_student(id, name) values(1, 'wang')"),
                     1);
    assert_int_equal(execute_update(planner, tx,
                                    "insert into update_student(id, name) values(2, 'li')"),
                     1);
}

static StudentRow find_student_by_id(MetadataMgr *metadata, Transaction *tx, int expectedId) {
    StudentRow row = {0};
    CString *tableName = CStringCreateFromCStr(TEST_TABLE_NAME);
    CString *idField = CStringCreateFromCStr("id");
    CString *nameField = CStringCreateFromCStr("name");
    Layout *layout = MetadataMgrGetLayout(metadata, tableName, tx);
    assert_non_null(layout);

    TableScan *tableScan = TableScanInit(tx, tableName, layout);
    Scan *scan = ScanInit(tableScan, SCAN_TABLE_CODE);
    assert_non_null(scan);

    while (scan->next(scan)) {
        int currentId = scan->getInt(scan, idField);
        if (currentId == expectedId) {
            const char *currentName = scan->getString(scan, nameField);
            row.found = true;
            row.id = currentId;
            snprintf(row.name, sizeof(row.name), "%s", currentName);
            break;
        }
    }

    scan->close(scan);
    free(scan);
    CStringDestroy(nameField);
    CStringDestroy(idField);
    CStringDestroy(tableName);
    return row;
}

static int count_students(MetadataMgr *metadata, Transaction *tx) {
    int count = 0;
    CString *tableName = CStringCreateFromCStr(TEST_TABLE_NAME);
    Layout *layout = MetadataMgrGetLayout(metadata, tableName, tx);
    assert_non_null(layout);

    TableScan *tableScan = TableScanInit(tx, tableName, layout);
    Scan *scan = ScanInit(tableScan, SCAN_TABLE_CODE);
    assert_non_null(scan);

    while (scan->next(scan)) {
        count++;
    }

    scan->close(scan);
    free(scan);
    CStringDestroy(tableName);
    return count;
}

static void test_update_planner_create_table_and_insert(void **state) {
    UpdateTestContext *ctx = *state;
    Transaction *tx = new_transaction(ctx);
    MetadataMgr *metadata = MetadataMgrInit(true, tx);
    Planner *planner = new_update_planner(metadata);
    CString *tableName = CStringCreateFromCStr(TEST_TABLE_NAME);
    CString *idField = CStringCreateFromCStr("id");
    CString *nameField = CStringCreateFromCStr("name");

    create_update_student_table(planner, tx);
    Layout *layout = MetadataMgrGetLayout(metadata, tableName, tx);

    assert_non_null(layout);
    assert_true(SchemaHasField(layout->schema, idField));
    assert_true(SchemaHasField(layout->schema, nameField));
    assert_int_equal(SchemaType(layout->schema, idField), FILE_INFO_CODE_INTEGER);
    assert_int_equal(SchemaType(layout->schema, nameField), FILE_INFO_CODE_VARCHAR);
    assert_int_equal(SchemaLength(layout->schema, nameField), 20);

    insert_update_student_rows(planner, tx);
    assert_int_equal(count_students(metadata, tx), 2);

    StudentRow first = find_student_by_id(metadata, tx, 1);
    StudentRow second = find_student_by_id(metadata, tx, 2);
    assert_true(first.found);
    assert_true(second.found);
    assert_string_equal(first.name, "wang");
    assert_string_equal(second.name, "li");

    TransactionCommit(tx);
    assert_int_equal(tx->code, TX_TRANSACTION_COMMIT);

    CStringDestroy(nameField);
    CStringDestroy(idField);
    CStringDestroy(tableName);
}

static void test_update_planner_modify_and_delete(void **state) {
    UpdateTestContext *ctx = *state;
    Transaction *tx = new_transaction(ctx);
    MetadataMgr *metadata = MetadataMgrInit(true, tx);
    Planner *planner = new_update_planner(metadata);

    create_update_student_table(planner, tx);
    insert_update_student_rows(planner, tx);

    assert_int_equal(execute_update(planner, tx,
                                    "update update_student set name = 'newwang' where id = 1"),
                     1);
    StudentRow updated = find_student_by_id(metadata, tx, 1);
    assert_true(updated.found);
    assert_string_equal(updated.name, "newwang");

    assert_int_equal(execute_update(planner, tx,
                                    "delete from update_student where id = 2"),
                     1);
    StudentRow deleted = find_student_by_id(metadata, tx, 2);
    assert_false(deleted.found);
    assert_int_equal(count_students(metadata, tx), 1);

    TransactionCommit(tx);
    assert_int_equal(tx->code, TX_TRANSACTION_COMMIT);
}

static void test_update_planner_create_view_and_index(void **state) {
    UpdateTestContext *ctx = *state;
    Transaction *tx = new_transaction(ctx);
    MetadataMgr *metadata = MetadataMgrInit(true, tx);
    Planner *planner = new_update_planner(metadata);
    CString *viewName = CStringCreateFromCStr("update_view");
    CString *tableName = CStringCreateFromCStr(TEST_TABLE_NAME);
    CString *idField = CStringCreateFromCStr("id");

    create_update_student_table(planner, tx);

    assert_int_equal(execute_update(planner, tx,
                                    "create view update_view as select id, name from update_student"),
                     0);
    CString *viewDef = MetadataMgrGetViewDef(metadata, viewName, tx);
    assert_non_null(viewDef);
    assert_string_equal(CStringGetPtr(viewDef), "select id, name from update_student ;");

    assert_int_equal(execute_update(planner, tx,
                                    "create index idx_upd_id on update_student(id)"),
                     0);
    map_IndexInfo_t *indexInfoMap = MetadataManagerGetIndexInfo(metadata, tableName, tx);
    assert_non_null(indexInfoMap);
    assert_int_equal(indexInfoMap->base.nnodes, 1);
    IndexInfo *indexInfo = map_get(indexInfoMap, CStringGetPtr(idField));
    assert_non_null(indexInfo);
    assert_string_equal(CStringGetPtr(indexInfo->idxname), "idx_upd_id");
    assert_string_equal(CStringGetPtr(indexInfo->fldname), "id");

    TransactionCommit(tx);
    assert_int_equal(tx->code, TX_TRANSACTION_COMMIT);

    map_deinit(indexInfoMap);
    free(indexInfoMap);
    CStringDestroy(viewDef);
    CStringDestroy(idField);
    CStringDestroy(tableName);
    CStringDestroy(viewName);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_update_planner_create_table_and_insert,
                                        setup_update_context,
                                        teardown_update_context),
        cmocka_unit_test_setup_teardown(test_update_planner_modify_and_delete,
                                        setup_update_context,
                                        teardown_update_context),
        cmocka_unit_test_setup_teardown(test_update_planner_create_view_and_index,
                                        setup_update_context,
                                        teardown_update_context),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
