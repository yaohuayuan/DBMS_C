#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdint.h>
#include "PredParser.h"
#include <stdlib.h>

// 测试 PredParser 结构体的基本功能
static void test_pred_parser_struct(void **state) {
    (void)state;

    // 创建一个简单的 PredParser 实例
    PredParser *predParser = (PredParser *)malloc(sizeof(PredParser));
    
    // 验证结构体的字段初始化
    assert_non_null(predParser);
    predParser->lexer = NULL;
    
    // 验证字段值
    assert_null(predParser->lexer);
    
    // 释放内存
    free(predParser);
}

// 主函数：注册所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_pred_parser_struct),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}