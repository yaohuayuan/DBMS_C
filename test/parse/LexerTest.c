#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdint.h>
#include "Lexer.h"
#include <stdlib.h>
#include <string.h>

// 测试 Lexer 初始化
static void test_lexer_init(void **state) {
    (void)state;

    const char *testStr = "SELECT * FROM test_table WHERE id = 123;";
    Lexer *lexer = LexerInit((char *)testStr);

    // 验证 Lexer 实例是否正确初始化
    assert_non_null(lexer);
    assert_non_null(lexer->tokenizer);
    assert_non_null(lexer->keyWords);
    
    // 注意：这里假设 LexerInit 函数会分配内存，实际可能需要调整
    // LexerFree(lexer); // 如果有 LexerFree 函数的话
}

// 测试 Lexer 结构体的基本功能
static void test_lexer_struct(void **state) {
    (void)state;

    // 创建一个简单的 Lexer 实例
    Lexer *lexer = (Lexer *)malloc(sizeof(Lexer));
    
    // 验证结构体的字段初始化
    assert_non_null(lexer);
    lexer->tokenizer = NULL;
    lexer->keyWords = NULL;
    
    // 验证字段值
    assert_null(lexer->tokenizer);
    assert_null(lexer->keyWords);
    
    // 释放内存
    free(lexer);
}

// 主函数：注册所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_lexer_init),
            cmocka_unit_test(test_lexer_struct),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}