#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "Predicate.h"
#include "Term.h"
#include "Expression.h"
#include "Constant.h"

// 测试Predicate初始化
static void test_predicate_init(void **state) {
    Constant *constant = ConstantInitInt(42);
    Expression *lhs = ExpressionInitFieldName("test_field");
    Expression *rhs = ExpressionInitConstant(constant);
    Term *term = TermInit(lhs, rhs);
    Predicate *predicate = PredicateInit(term);
    
    assert_non_null(predicate);
    assert_non_null(predicate->terms);
    
    // 清理资源
    // 注意：直接使用free释放内存，因为没有提供专门的释放函数
    free(predicate->terms);
    free(predicate);
    free(term);
    ExpressionFree(lhs);
    ExpressionFree(rhs);
    ConstantFree(constant);
}

// 测试Predicate连接
static void test_predicate_conjoin_with(void **state) {
    // 创建第一个predicate
    Constant *constant1 = ConstantInitInt(42);
    Expression *lhs1 = ExpressionInitFieldName("field1");
    Expression *rhs1 = ExpressionInitConstant(constant1);
    Term *term1 = TermInit(lhs1, rhs1);
    Predicate *predicate1 = PredicateInit(term1);
    
    // 创建第二个predicate
    Constant *constant2 = ConstantInitString("test");
    Expression *lhs2 = ExpressionInitFieldName("field2");
    Expression *rhs2 = ExpressionInitConstant(constant2);
    Term *term2 = TermInit(lhs2, rhs2);
    Predicate *predicate2 = PredicateInit(term2);
    
    // 连接两个predicate
    PredicateConjoinWith(predicate1, predicate2);
    
    // 清理资源
    free(predicate1->terms);
    free(predicate1);
    free(predicate2->terms);
    free(predicate2);
    free(term1);
    free(term2);
    ExpressionFree(lhs1);
    ExpressionFree(rhs1);
    ExpressionFree(lhs2);
    ExpressionFree(rhs2);
    ConstantFree(constant1);
    ConstantFree(constant2);
}

// 测试Predicate转换为字符串
static void test_predicate_to_string(void **state) {
    Constant *constant = ConstantInitInt(42);
    Expression *lhs = ExpressionInitFieldName("test_field");
    Expression *rhs = ExpressionInitConstant(constant);
    Term *term = TermInit(lhs, rhs);
    Predicate *predicate = PredicateInit(term);
    
    char *str = PredicateToString(predicate);
    assert_non_null(str);
    assert_string_equal(str, "test_field=42");
    
    // 清理资源
    free(str);
    free(predicate->terms);
    free(predicate);
    free(term);
    ExpressionFree(lhs);
    ExpressionFree(rhs);
    ConstantFree(constant);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_predicate_init),
        cmocka_unit_test(test_predicate_conjoin_with),
        cmocka_unit_test(test_predicate_to_string),
    };
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}
