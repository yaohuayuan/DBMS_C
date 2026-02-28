//
// Created by Lenovo on 2025/8/12.
//
#include <stddef.h>
#include <cmocka.h>

#include "ConcurrencyManager.h"
#include "LockTable.h"

static void test_lock_upgrade_success(void **state) {
    (void) state; // 未用到

    // 初始化事务的并发管理器
    ConCurrencyManager *mgr = ConCurrencyManagerInit();
    assert_non_null(mgr);

    // 构造一个 BlockID
    BlockID *blk = BlockIDInit(CStringCreateFromCStr("testfile"), 1);

    // Step 1: 先上 S 锁
    ConCurrencyManagerSLock(mgr, blk);

    // 验证确实有 S 锁
    char **lockType = map_get(mgr->mapStr, CStringGetPtr(BlockID2CString(blk)));
    assert_non_null(lockType);
    assert_string_equal(*lockType, "S");

    // Step 2: 升级到 X 锁
    ConCurrencyManagerXLock(mgr, blk);

    // Step 3: 验证已经是 X 锁
    lockType = map_get(mgr->mapStr, CStringGetPtr(BlockID2CString(blk)));
    assert_non_null(lockType);
    assert_string_equal(*lockType, "X");
    assert_true(ConCurrencyManagerHasXLock(mgr, blk));

    // 释放锁
    ConCurrencyManagerRelease(mgr);

    free(mgr);
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_lock_upgrade_success),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
