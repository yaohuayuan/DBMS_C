#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdint.h>
#include "Expression.h"
#include "Constant.h"
#include "CString.h"
#include <stdlib.h>
#include <string.h>

// 测试创建常量表达式
static void test_expression_init_constant(void **state) {
    (void)state;

    // 创建一个整数常量
    Constant *constant = ConstantInitInt(42);
    Expression *expr = ExpressionInitConstant(constant);

    // 验证表达式是否正确创建
    assert_non_null(expr);
    assert_non_null(expr->val);
    assert_null(expr->fldname);
    assert_true(ConstantCompareTo(expr->val, constant) == 0);
    
    // 验证表达式类型
    assert_false(ExpressionIsFieldName(expr));
    assert_non_null(ExpressionAsConstant(expr));
    assert_null(ExpressionAsFieldName(expr));
    
    // 释放资源
    ExpressionFree(expr);
    ConstantFree(constant);
}

// 测试创建字段名表达式
static void test_expression_init_field_name(void **state) {
    (void)state;

    const char *fieldName = "test_field";
    Expression *expr = ExpressionInitFieldName(fieldName);

    // 验证表达式是否正确创建
    assert_non_null(expr);
    assert_null(expr->val);
    assert_non_null(expr->fldname);
    assert_string_equal(CStringGetPtr(expr->fldname), fieldName);
    
    // 验证表达式类型
    assert_true(ExpressionIsFieldName(expr));
    assert_null(ExpressionAsConstant(expr));
    assert_non_null(ExpressionAsFieldName(expr));
    assert_string_equal(ExpressionAsFieldName(expr), fieldName);
    
    // 释放资源
    ExpressionFree(expr);
}

// 测试表达式转换为字符串
static void test_expression_to_string(void **state) {
    (void)state;

    // 测试常量表达式转换为字符串
    Constant *constant = ConstantInitInt(100);
    Expression *constExpr = ExpressionInitConstant(constant);
    char *constStr = ExpressionToString(constExpr);
    assert_non_null(constStr);
    assert_string_equal(constStr, "100");
    free(constStr);
    ExpressionFree(constExpr);
    ConstantFree(constant);
    
    // 测试字段名表达式转换为字符串
    Expression *fieldExpr = ExpressionInitFieldName("test_field");
    char *fieldStr = ExpressionToString(fieldExpr);
    assert_non_null(fieldStr);
    assert_string_equal(fieldStr, "test_field");
    free(fieldStr);
    ExpressionFree(fieldExpr);
}

// 主函数：注册所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_expression_init_constant),
            cmocka_unit_test(test_expression_init_field_name),
            cmocka_unit_test(test_expression_to_string),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}