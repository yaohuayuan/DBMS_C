//
// Created by Lenovo on 2025/7/31.
//

#include <stddef.h>
#include <cmocka.h>
#include "FileManager.h"
#include "LogManager.h"
#include "BufferManager.h"
#include "Transaction.h"
#include "LRU/LRUCore.h"
#include "LRU/LRUPolicy.h"

static void test_tx_commit_and_rollback(void **state) {
    (void) state;

    FileManager *fileManager = FileManagerInit(CStringCreateFromCStr("TXTest"), 400);
    LogManager *logManager = LogManagerInit(fileManager, CStringCreateFromCStr("test.log"));
    ReplacementPolicy *replacement_policy = LRUPolicyCreate(8);
    BufferManager *bufferManager = BufferManagerInit(fileManager, logManager, 8,replacement_policy);

    BlockID *blockId = BlockIDInit(CStringCreateFromCStr("testfile"),1);

    // tx1: 写入初始数据
    Transaction *tx1 = TransactionInit(fileManager, logManager, bufferManager);
    TransactionPin(tx1, blockId);
    TransactionSetInt(tx1, blockId, 0, 1, false);
    TransactionSetString(tx1, blockId, 40, CStringCreateFromCStr("one"), false);
    TransactionCommit(tx1);

    // tx2: 读取验证
    Transaction *tx2 = TransactionInit(fileManager, logManager, bufferManager);
    TransactionPin(tx2, blockId);
    int intVal = TransactionGetInt(tx2, blockId, 0);
    CString *strVal = TransactionGetString(tx2, blockId, 40);
    assert_int_equal(intVal, 1);
    assert_string_equal(CStringGetPtr(strVal), "one");
    TransactionCommit(tx2);

    // tx3: 修改后回滚
    Transaction *tx3 = TransactionInit(fileManager, logManager, bufferManager);
    TransactionPin(tx3, blockId);
    TransactionSetInt(tx3, blockId, 0, 2, true); // set 2, rollback
    TransactionRollback(tx3);

    // tx4: 再次读取应为回滚前的值
    Transaction *tx4 = TransactionInit(fileManager, logManager, bufferManager);
    TransactionPin(tx4, blockId);
    int intVal2 = TransactionGetInt(tx4, blockId, 0);
    CString *strVal2 = TransactionGetString(tx4, blockId, 40);
    assert_int_equal(intVal2, 1);
    assert_string_equal(CStringGetPtr(strVal2), "one");
    TransactionCommit(tx4);  // 添加这一行！
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_tx_commit_and_rollback),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
