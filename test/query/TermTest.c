#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "Term.h"
#include "Expression.h"
#include "Constant.h"
#include "CString.h"
#include "Schema.h"

// 测试Term初始化
static void test_term_init(void **state) {
    Constant *constant = ConstantInitInt(42);
    Expression *lhs = ExpressionInitFieldName("test_field");
    Expression *rhs = ExpressionInitConstant(constant);
    Term *term = TermInit(lhs, rhs);
    
    assert_non_null(term);
    assert_non_null(term->lhs);
    assert_non_null(term->rhs);
    
    // 清理资源
    free(term);
    ExpressionFree(lhs);
    ExpressionFree(rhs);
    ConstantFree(constant);
}

// 测试Term转换为字符串
static void test_term_to_string(void **state) {
    Constant *constant = ConstantInitInt(42);
    Expression *lhs = ExpressionInitFieldName("test_field");
    Expression *rhs = ExpressionInitConstant(constant);
    Term *term = TermInit(lhs, rhs);
    
    char *str = TermToString(term);
    assert_non_null(str);
    assert_string_equal(str, "test_field=42");
    
    // 清理资源
    free(str);
    free(term);
    ExpressionFree(lhs);
    ExpressionFree(rhs);
    ConstantFree(constant);
}

// 测试Term是否适用于给定模式
static void test_term_applies_to(void **state) {
    // 创建一个schema
    Schema *schema = SchemaInit();
    CString *fieldName = CStringCreateFromCStr("test_field");
    SchemaAddIntField(schema, fieldName);
    
    // 创建term
    Constant *constant = ConstantInitInt(42);
    Expression *lhs = ExpressionInitFieldName("test_field");
    Expression *rhs = ExpressionInitConstant(constant);
    Term *term = TermInit(lhs, rhs);
    
    // 测试term是否适用于schema
    assert_true(TermAppliesTo(term, schema));
    
    // 清理资源
    free(term);
    ExpressionFree(lhs);
    ExpressionFree(rhs);
    ConstantFree(constant);
    CStringDestroy(fieldName);
    SchemaFree(schema);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_term_init),
        cmocka_unit_test(test_term_to_string),
        cmocka_unit_test(test_term_applies_to),
    };
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}
