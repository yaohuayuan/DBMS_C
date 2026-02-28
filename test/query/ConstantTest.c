#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdint.h>
#include "Constant.h"
#include <stdlib.h>
#include <string.h>

// 测试整数常量的初始化和获取值
static void test_constant_int_init_and_get(void **state) {
    (void)state;

    Constant *c = ConstantInitInt(42);

    assert_non_null(c);
    assert_true(c->isInt);
    assert_int_equal(ConstantAsInt(c), 42);
    
    ConstantFree(c);
}

// 测试字符串常量的初始化和获取值
static void test_constant_string_init_and_get(void **state) {
    (void)state;

    const char *testStr = "test string";
    Constant *c = ConstantInitString(testStr);

    assert_non_null(c);
    assert_false(c->isInt);
    assert_string_equal(ConstantAsString(c), testStr);
    
    ConstantFree(c);
}

// 测试常量比较
static void test_constant_compare(void **state) {
    (void)state;

    Constant *c1 = ConstantInitInt(10);
    Constant *c2 = ConstantInitInt(20);
    Constant *c3 = ConstantInitInt(10);
    
    // 比较整数常量
    assert_int_equal(ConstantCompareTo(c1, c2), -1);
    assert_int_equal(ConstantCompareTo(c2, c1), 1);
    assert_int_equal(ConstantCompareTo(c1, c3), 0);
    
    ConstantFree(c1);
    ConstantFree(c2);
    ConstantFree(c3);
    
    // 比较字符串常量
    Constant *s1 = ConstantInitString("apple");
    Constant *s2 = ConstantInitString("banana");
    Constant *s3 = ConstantInitString("apple");
    
    assert_int_equal(ConstantCompareTo(s1, s2), -1);
    assert_int_equal(ConstantCompareTo(s2, s1), 1);
    assert_int_equal(ConstantCompareTo(s1, s3), 0);
    
    ConstantFree(s1);
    ConstantFree(s2);
    ConstantFree(s3);
}

// 测试常量相等性
static void test_constant_equals(void **state) {
    (void)state;

    Constant *c1 = ConstantInitInt(50);
    Constant *c2 = ConstantInitInt(50);
    Constant *c3 = ConstantInitInt(60);
    
    // 比较整数常量相等性
    assert_true(ConstantEquals(c1, c2));
    assert_false(ConstantEquals(c1, c3));
    
    ConstantFree(c1);
    ConstantFree(c2);
    ConstantFree(c3);
    
    // 比较字符串常量相等性
    Constant *s1 = ConstantInitString("test");
    Constant *s2 = ConstantInitString("test");
    Constant *s3 = ConstantInitString("different");
    
    assert_true(ConstantEquals(s1, s2));
    assert_false(ConstantEquals(s1, s3));
    
    ConstantFree(s1);
    ConstantFree(s2);
    ConstantFree(s3);
}

// 测试常量哈希码
static void test_constant_hash_code(void **state) {
    (void)state;

    Constant *c1 = ConstantInitInt(100);
    Constant *c2 = ConstantInitInt(100);
    Constant *c3 = ConstantInitInt(200);
    
    // 相同值的常量应该有相同的哈希码
    assert_int_equal(ConstantHashCode(c1), ConstantHashCode(c2));
    // 不同值的常量应该有不同的哈希码
    assert_false(ConstantHashCode(c1) == ConstantHashCode(c3));
    
    ConstantFree(c1);
    ConstantFree(c2);
    ConstantFree(c3);
    
    // 测试字符串常量哈希码
    Constant *s1 = ConstantInitString("hash");
    Constant *s2 = ConstantInitString("hash");
    Constant *s3 = ConstantInitString("different");
    
    assert_int_equal(ConstantHashCode(s1), ConstantHashCode(s2));
    assert_false(ConstantHashCode(s1) == ConstantHashCode(s3));
    
    ConstantFree(s1);
    ConstantFree(s2);
    ConstantFree(s3);
}

// 测试常量转换为字符串
static void test_constant_to_string(void **state) {
    (void)state;

    // 测试整数常量转换为字符串
    Constant *c = ConstantInitInt(123);
    char *str = ConstantToString(c);
    
    assert_non_null(str);
    assert_string_equal(str, "123");
    
    free(str);
    ConstantFree(c);
    
    // 测试字符串常量转换为字符串
    Constant *s = ConstantInitString("hello");
    str = ConstantToString(s);
    
    assert_non_null(str);
    assert_string_equal(str, "hello");
    
    free(str);
    ConstantFree(s);
}

// 主函数：注册所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_constant_int_init_and_get),
            cmocka_unit_test(test_constant_string_init_and_get),
            cmocka_unit_test(test_constant_compare),
            cmocka_unit_test(test_constant_equals),
            cmocka_unit_test(test_constant_hash_code),
            cmocka_unit_test(test_constant_to_string),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}