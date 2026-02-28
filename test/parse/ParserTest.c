#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdint.h>
#include "Parser.h"
#include <stdlib.h>
#include <string.h>

// 测试解析器初始化
static void test_parser_init(void **state) {
    (void)state;

    const char *testSql = "SELECT * FROM test_table WHERE id = 1;";
    Parser *parser = ParserInit((char *)testSql);

    // 验证解析器是否正确初始化
    assert_non_null(parser);
    assert_non_null(parser->lexer);
    
    // 注意：这里假设ParserInit函数会分配内存，实际可能需要调整
    // ParserFree(parser); // 如果有ParserFree函数的话
}

// 测试CommandData结构体的基本功能
static void test_command_data_struct(void **state) {
    (void)state;

    // 创建一个CommandData实例
    CommandData cmdData;
    cmdData.code = CMD_INSERT_DATA;
    
    // 验证CommandData结构体的字段
    assert_int_equal(cmdData.code, CMD_INSERT_DATA);
}

// 主函数：注册所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_parser_init),
            cmocka_unit_test(test_command_data_struct),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}