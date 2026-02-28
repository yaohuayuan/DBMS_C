#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include "DBMS.h"

// 测试 SimpleDBInit 函数
static void test_simpledb_init(void **state) {
    (void)state;
    
    // 使用默认参数初始化
    SimpleDB *db = SimpleDBInit("test_db", SIMPLE_DB_INIT_VAL, SIMPLE_DB_INIT_VAL);
    
    assert_non_null(db);
    assert_non_null(db->fileManager);
    assert_non_null(db->logManager);
    assert_non_null(db->bufferManager);
    assert_non_null(db->metadataMgr);
    assert_non_null(db->planer);
    
    // TODO: 这里应该释放资源，但当前没有提供销毁函数
    // 可以考虑添加 SimpleDBDestroy 函数
}

// 测试 SimpleDataNewTX 函数
static void test_simpledata_new_tx(void **state) {
    (void)state;
    
    SimpleDB *db = SimpleDBInit("test_db", SIMPLE_DB_INIT_VAL, SIMPLE_DB_INIT_VAL);
    assert_non_null(db);
    
    Transaction *tx = SimpleDataNewTX(db);
    assert_non_null(tx);
    
    // 提交事务
    TransactionCommit(tx);
}

// 测试 SimpleDataNewTXName 函数
static void test_simpledata_new_tx_name(void **state) {
    (void)state;
    
    SimpleDB *db = SimpleDBInit("test_db", SIMPLE_DB_INIT_VAL, SIMPLE_DB_INIT_VAL);
    assert_non_null(db);
    
    char *name = "test_tx";
    Transaction *tx = SimpleDataNewTXName(db, &name);
    assert_non_null(tx);
    
    // 提交事务
    TransactionCommit(tx);
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_simpledb_init),
            cmocka_unit_test(test_simpledata_new_tx),
            cmocka_unit_test(test_simpledata_new_tx_name),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}