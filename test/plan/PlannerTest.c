#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdint.h>
#include "Planner.h"
#include <stdlib.h>

// 测试 Planner 结构体的基本功能
static void test_planner_struct(void **state) {
    (void)state;

    // 注意：由于 PlannerInit 依赖于其他组件，这里仅测试结构体的基本功能
    // 在实际项目中，应该使用模拟（mock）的组件来测试完整功能

    // 创建一个简单的 Planner 实例
    Planner *planner = (Planner *)malloc(sizeof(Planner));
    
    // 验证结构体的字段初始化
    assert_non_null(planner);
    planner->queryPlanner = NULL;
    planner->updatePlanner = NULL;
    
    // 验证字段值
    assert_null(planner->queryPlanner);
    assert_null(planner->updatePlanner);
    
    // 释放内存
    free(planner);
}

// 主函数：注册所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_planner_struct),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}