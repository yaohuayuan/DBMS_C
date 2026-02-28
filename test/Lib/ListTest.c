#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdint.h>
#include "List.h"
#include "CString.h"

// 测试 ListInit 和 ListFree
static void test_list_init_and_free(void **state) {
    (void)state;

    List *list = ListInit(LIST_TYPE_STRING, NULL, NULL, NULL);

    assert_non_null(list);
    assert_int_equal(list->size, 0);
    assert_int_equal(list->type, LIST_TYPE_STRING);
    assert_null(list->head);

    ListFree(list);
}

// 测试 ListAppend 和 ListContains
static void test_list_append_and_contains(void **state) {
    (void)state;

    List *list = ListInit(LIST_TYPE_STRING, NULL, NULL, NULL);
    
    // 创建测试字符串
    CString *str1 = CStringCreateFromCStr("test1");
    CString *str2 = CStringCreateFromCStr("test2");
    CString *str3 = CStringCreateFromCStr("test3");
    
    // 添加元素
    ListAppend(list, str1);
    assert_int_equal(list->size, 1);
    assert_true(ListContains(list, str1));
    
    ListAppend(list, str2);
    assert_int_equal(list->size, 2);
    assert_true(ListContains(list, str2));
    
    // 测试不包含的元素
    assert_false(ListContains(list, str3));
    
    // 由于ListFree会调用内部的string_free，我们不需要手动销毁str1和str2
    ListFree(list);
    CStringDestroy(str3);
}

// 测试 ListRemoveByValue
static void test_list_remove_by_value(void **state) {
    (void)state;

    List *list = ListInit(LIST_TYPE_STRING, NULL, NULL, NULL);
    
    CString *str1 = CStringCreateFromCStr("remove1");
    CString *str2 = CStringCreateFromCStr("remove2");
    
    ListAppend(list, str1);
    ListAppend(list, str2);
    
    assert_int_equal(list->size, 2);
    assert_true(ListContains(list, str1));
    
    ListRemoveByValue(list, str1);
    assert_int_equal(list->size, 1);
    assert_false(ListContains(list, str1));
    assert_true(ListContains(list, str2));
    
    ListFree(list);
}

// 测试 ListAddAll
static void test_list_add_all(void **state) {
    (void)state;

    List *list1 = ListInit(LIST_TYPE_STRING, NULL, NULL, NULL);
    List *list2 = ListInit(LIST_TYPE_STRING, NULL, NULL, NULL);
    
    CString *str1 = CStringCreateFromCStr("list1-1");
    CString *str2 = CStringCreateFromCStr("list1-2");
    CString *str3 = CStringCreateFromCStr("list2-1");
    CString *str4 = CStringCreateFromCStr("list2-2");
    
    ListAppend(list1, str1);
    ListAppend(list1, str2);
    
    ListAppend(list2, str3);
    ListAppend(list2, str4);
    
    assert_int_equal(list1->size, 2);
    assert_int_equal(list2->size, 2);
    
    ListAddAll(list1, list2);
    
    assert_int_equal(list1->size, 4);
    assert_true(ListContains(list1, str1));
    assert_true(ListContains(list1, str2));
    assert_true(ListContains(list1, str3));
    assert_true(ListContains(list1, str4));
    
    ListFree(list1);
    ListFree(list2);
}

// 测试 ListRemoveByIndex
static void test_list_remove_by_index(void **state) {
    (void)state;

    List *list = ListInit(LIST_TYPE_STRING, NULL, NULL, NULL);
    
    CString *str1 = CStringCreateFromCStr("index0");
    CString *str2 = CStringCreateFromCStr("index1");
    CString *str3 = CStringCreateFromCStr("index2");
    
    ListAppend(list, str1);
    ListAppend(list, str2);
    ListAppend(list, str3);
    
    assert_int_equal(list->size, 3);
    
    // 移除索引1的元素
    void *removed = ListRemoveByIndex(list, 1);
    assert_non_null(removed);
    assert_int_equal(list->size, 2);
    assert_false(ListContains(list, str2));
    CStringDestroy((CString *)removed);
    
    // 移除索引0的元素
    removed = ListRemoveByIndex(list, 0);
    assert_non_null(removed);
    assert_int_equal(list->size, 1);
    assert_false(ListContains(list, str1));
    CStringDestroy((CString *)removed);
    
    // 移除最后一个元素
    removed = ListRemoveByIndex(list, 0);
    assert_non_null(removed);
    assert_int_equal(list->size, 0);
    assert_false(ListContains(list, str3));
    CStringDestroy((CString *)removed);
    
    // 测试无效索引
    removed = ListRemoveByIndex(list, 100);
    assert_null(removed);
    
    ListFree(list);
}

// 主函数：注册所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_list_init_and_free),
            cmocka_unit_test(test_list_append_and_contains),
            cmocka_unit_test(test_list_remove_by_value),
            cmocka_unit_test(test_list_add_all),
            cmocka_unit_test(test_list_remove_by_index),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}