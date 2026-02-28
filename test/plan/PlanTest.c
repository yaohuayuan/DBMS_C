#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdint.h>
#include "Plan.h"
#include <stdlib.h>

// 测试 PlanInit 函数
static void test_plan_init(void **state) {
    (void)state;

    // 创建一个简单的数据结构体用于测试
    struct TestData {
        int value;
    } testData;
    testData.value = 42;

    // 初始化一个 Plan 实例
    Plan *plan = PlanInit(&testData, PLAN_TABLE_CODE);

    // 验证 Plan 实例是否正确初始化
    assert_non_null(plan);
    assert_int_equal(plan->code, PLAN_TABLE_CODE);
    
    // 释放 Plan 实例（注意：这里假设 PlanInit 函数会分配内存，实际可能需要调整）
    // PlanFree(plan); // 如果有 PlanFree 函数的话
}

// 测试 Plan 结构体的基本功能
static void test_plan_struct(void **state) {
    (void)state;

    // 创建测试数据
    struct TestData {
        int value;
    } testData;
    testData.value = 84;

    // 初始化 Plan 实例
    Plan *plan = PlanInit(&testData, PLAN_SELECT_CODE);

    // 验证 Plan 结构体的字段
    assert_non_null(plan);
    assert_int_equal(plan->code, PLAN_SELECT_CODE);
    
    // 释放 Plan 实例
    // PlanFree(plan); // 如果有 PlanFree 函数的话
}

// 主函数：注册所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_plan_init),
            cmocka_unit_test(test_plan_struct),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}