#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdint.h>
#include "StreamTokenizer.h"

// 测试 StreamTokenizerInit 和基本功能
static void test_stream_tokenizer_init(void **state) {
    (void)state;

    char *testString = "SELECT * FROM table WHERE id = 123;";
    StreamTokenizer *tokenizer = StreamTokenizerInit(testString);

    assert_non_null(tokenizer);
    assert_non_null(tokenizer->data);
    assert_non_null(tokenizer->keySet);
    assert_non_null(tokenizer->streamTokenizerTokenId);
    
    // 测试初始状态
    assert_int_equal(tokenizer->streamTokenizerTokenId->len, 0);
    assert_int_equal(tokenizer->streamTokenizerTokenId->endLen, 0);
    
    // 不需要释放tokenizer，因为目前没有提供释放函数
}

// 测试 StreamTokenizerNext 解析简单字符串
static void test_stream_tokenizer_next(void **state) {
    (void)state;

    char *testString = "hello world 123";
    StreamTokenizer *tokenizer = StreamTokenizerInit(testString);

    assert_non_null(tokenizer);
    
    // 初始搜索准备
    StreamTokenizerSearchReady(tokenizer);
    
    // 解析第一个令牌 "hello"
    StreamTokenizerNext(tokenizer);
    assert_int_equal(tokenizer->type, TOKEN_WORD);
    
    // 解析第二个令牌 "world"
    StreamTokenizerNext(tokenizer);
    assert_int_equal(tokenizer->type, TOKEN_WORD);
    
    // 解析第三个令牌 "123"
    StreamTokenizerNext(tokenizer);
    assert_int_equal(tokenizer->type, TOKEN_INT_CONSTANT);
}

// 测试 StreamTokenizerLowerCaseMode
static void test_stream_tokenizer_lowercase_mode(void **state) {
    (void)state;

    char *testString = "HELLO WORLD";
    StreamTokenizer *tokenizer = StreamTokenizerInit(testString);

    assert_non_null(tokenizer);
    
    // 设置小写模式
    StreamTokenizerLowerCaseMode(tokenizer);
    StreamTokenizerSearchReady(tokenizer);
    
    // 解析第一个令牌，应该转换为小写
    StreamTokenizerNext(tokenizer);
    assert_int_equal(tokenizer->type, TOKEN_WORD);
}

// 测试 StreamTokenizerWordChars 设置自定义单词字符
static void test_stream_tokenizer_word_chars(void **state) {
    (void)state;

    char *testString = "hello-world_123";
    StreamTokenizer *tokenizer = StreamTokenizerInit(testString);

    assert_non_null(tokenizer);
    
    // 设置连字符和下划线为单词字符
    StreamTokenizerWordChars(tokenizer, '-', '-');
    StreamTokenizerWordChars(tokenizer, '_', '_');
    StreamTokenizerSearchReady(tokenizer);
    
    // 解析整个字符串应该作为一个单词
    StreamTokenizerNext(tokenizer);
    StreamTokenizerType(tokenizer);
    assert_int_equal(tokenizer->type, TOKEN_WORD);
}

// 测试 StreamTokenizerOrdinaryChar
static void test_stream_tokenizer_ordinary_char(void **state) {
    (void)state;

    char *testString = "hello,world";
    StreamTokenizer *tokenizer = StreamTokenizerInit(testString);

    assert_non_null(tokenizer);
    
    // 设置逗号为普通字符
    StreamTokenizerOrdinaryChar(tokenizer, ',');
    StreamTokenizerSearchReady(tokenizer);
    
    // 解析第一个令牌，应该包含逗号
    StreamTokenizerNext(tokenizer);
    StreamTokenizerType(tokenizer);
    assert_int_equal(tokenizer->type, TOKEN_WORD);
}

// 主函数：注册所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_stream_tokenizer_init),
            cmocka_unit_test(test_stream_tokenizer_next),
            cmocka_unit_test(test_stream_tokenizer_lowercase_mode),
            cmocka_unit_test(test_stream_tokenizer_word_chars),
            cmocka_unit_test(test_stream_tokenizer_ordinary_char),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}