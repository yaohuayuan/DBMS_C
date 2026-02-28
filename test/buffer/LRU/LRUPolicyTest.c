//
// Created by Lenovo on 2026/1/14.
//
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdlib.h>
#include "LRU/LRUPolicy.h"
#include "ReplacementPolicy.h"

// 测试LRU创建与销毁
static void test_LRUPolicyCreate_destroy(void **state) {
    (void) state;

    ReplacementPolicy* policy = LRUPolicyCreate(10);
    assert_non_null(policy);
    assert_non_null(policy->impl);

    policy->destroy(policy->impl);
}

// 测试LRU记录访问并返回淘汰帧
static void test_LRUPolicy_record_access_evict(void **state) {
    (void) state;

    ReplacementPolicy* policy = LRUPolicyCreate(3);

    // 模拟访问 1, 2, 3
    policy->record_access(policy->impl, 1);
    policy->record_access(policy->impl, 2);
    policy->record_access(policy->impl, 3);

    // 访问 2, 1 让 3 成为 LRU
    policy->record_access(policy->impl, 2);
    policy->record_access(policy->impl, 1);

    int victim = policy->evict(policy->impl);
    assert_int_equal(victim, 3);  // 3 是最久未访问的

    policy->destroy(policy->impl);
}

// 测试移除指定帧
static void test_LRUPolicy_remove(void **state) {
    (void) state;

    ReplacementPolicy* policy = LRUPolicyCreate(3);

    policy->record_access(policy->impl, 1);
    policy->record_access(policy->impl, 2);
    policy->record_access(policy->impl, 3);

    // 移除 2
    policy->remove(policy->impl, 2);

    // 现在访问 1,3 再淘汰应返回最久未访问的 1
    int victim = policy->evict(policy->impl);
    assert_int_equal(victim, 1);

    policy->destroy(policy->impl);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_LRUPolicyCreate_destroy),
        cmocka_unit_test(test_LRUPolicy_record_access_evict),
        cmocka_unit_test(test_LRUPolicy_remove),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
