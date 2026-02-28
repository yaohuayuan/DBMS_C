#include <stdio.h>
#include <string.h>
#include <assert.h> // 这个是标准C库的assert，不是cmocka的
#include <cmocka.h> // CMocka 的断言在这里

// ANSI 色彩打印（可选）
#define GREEN_TEXT "\033[0;32m"
#define RED_TEXT "\033[0;31m"
#define RESET_TEXT "\033[0m"

// 引入你的 CString.h 头文件
#include "CString.h"

static void test_create_and_get(void **state) {
    (void)state;  // Suppress unused parameter warning
    CString* cs = CStringCreateFromCStr("Hello");
    assert_non_null(cs);
    assert_int_equal(CStringGetLength(cs), 5);
    // 修正：CMocka 没有 assert_int_greater_or_equal
    assert_true(CStringGetCapacity(cs) >= 5); // 使用 assert_true 配合比较运算符
    assert_string_equal(CStringGetPtr(cs), "Hello");
    CStringDestroy(cs);
}

static void test_append_char_and_cstr(void **state) {
    (void)state;
    CString* cs = CStringCreate();
    CStringAppendChar(cs, 'A');
    CStringAppendCStr(cs, "BC");
    assert_string_equal(CStringGetPtr(cs), "ABC");
    assert_int_equal(CStringGetLength(cs), 3);
    CStringDestroy(cs);
}

static void test_compare_and_equal(void **state) {
    (void)state;
    CString* cs1 = CStringCreateFromCStr("abc");
    CString* cs2 = CStringCreateFromCStr("abc");
    CString* cs3 = CStringCreateFromCStr("abd");
    assert_true(CStringEqual(cs1, cs2));
    assert_false(CStringEqual(cs1, cs3));
    // 修正：CMocka 没有 assert_int_less_or_equal
    assert_true(CStringCompare(cs1, cs3) <= 0); // 使用 assert_true 配合比较运算符
    CStringDestroy(cs1);
    CStringDestroy(cs2);
    CStringDestroy(cs3);
}

static void test_substring_find_replace(void **state) {
    (void)state;
    CString* cs = CStringCreateFromCStr("Hello World");
    int index = CStringFind(cs, "World");
    assert_int_equal(index, 6);

    CString* sub = CStringSubstring(cs, 6, 5);
    assert_string_equal(CStringGetPtr(sub), "World");

    // 注意：CStringReplace 的返回值可能需要根据其实现来判断是否为1表示成功
    // 如果其返回值是替换的次数，那么 assert_int_equal(replaced, 1) 更精确
    int replaced = CStringReplace(cs, "World", "C");
    assert_true(replaced); // 假设替换成功返回非0，失败返回0
    assert_string_equal(CStringGetPtr(cs), "Hello C");

    CStringDestroy(sub);
    CStringDestroy(cs);
}

static void test_insert_remove_clear(void **state) {
    (void)state;
    CString* cs = CStringCreateFromCStr("HellWorld");
    assert_int_equal(CStringInsert(cs, 4, "o "), 1);  // Insert "o "
    assert_string_equal(CStringGetPtr(cs), "Hello World");

    assert_int_equal(CStringRemove(cs, 5, 1), 1);  // Remove space
    assert_string_equal(CStringGetPtr(cs), "HelloWorld");

    CStringClear(cs);
    assert_string_equal(CStringGetPtr(cs), "");
    assert_int_equal(CStringGetLength(cs), 0);

    CStringDestroy(cs);
}

static void test_case_conversion(void **state) {
    (void)state;
    CString* cs = CStringCreateFromCStr("HeLLo");
    CStringToLower(cs);
    assert_string_equal(CStringGetPtr(cs), "hello");
    CStringToUpper(cs);
    assert_string_equal(CStringGetPtr(cs), "HELLO");
    CStringDestroy(cs);
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_create_and_get),
            cmocka_unit_test(test_append_char_and_cstr),
            cmocka_unit_test(test_compare_and_equal),
            cmocka_unit_test(test_substring_find_replace),
            cmocka_unit_test(test_insert_remove_clear),
            cmocka_unit_test(test_case_conversion),
    };

    printf("Running CString tests...\n");

    return cmocka_run_group_tests(tests, NULL, NULL);
}