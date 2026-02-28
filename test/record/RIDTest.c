#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "RID.h"

// 测试RID初始化
static void test_rid_init(void **state) {
    RID *rid = RIDInit(10, 20);
    
    assert_non_null(rid);
    assert_int_equal(rid->BlockNum, 10);
    assert_int_equal(rid->Slot, 20);
    
    // 清理资源
    free(rid);
}

// 测试RID相等性
static void test_rid_equal(void **state) {
    RID *rid1 = RIDInit(10, 20);
    RID *rid2 = RIDInit(10, 20);
    RID *rid3 = RIDInit(20, 30);
    
    assert_true(RIDEqual(rid1, rid2));
    assert_false(RIDEqual(rid1, rid3));
    assert_false(RIDEqual(rid2, rid3));
    
    // 清理资源
    free(rid1);
    free(rid2);
    free(rid3);
}

// 测试RID转换为字符串
static void test_rid_to_string(void **state) {
    RID *rid = RIDInit(10, 20);
    char *str = RIDToString(rid);
    
    assert_non_null(str);
    assert_string_equal(str, "(10,20)");
    
    // 清理资源
    free(str);
    free(rid);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_rid_init),
        cmocka_unit_test(test_rid_equal),
        cmocka_unit_test(test_rid_to_string),
    };
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}
