//
// Created by Lenovo on 2025/6/24.
//
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include "CMap.h"
#include <string.h>
#include <stdlib.h>
#include "CString.h"
int compare_ints(const void *a, const void *b) {
    const int val_a = *(const int *)a;
    const int val_b = *(const int *)b;
    return (val_a > val_b) - (val_a < val_b);
}
// 测试用的字符串比较函数 (如果需要测试字符串CMap)

// 字符串释放函数 (如果需要测试字符串CMap)
void free_string_ptr(void *str_ptr) {
    // str_ptr 实际上是 void* 指向 char* 的指针
    CString *cs = (CString*)str_ptr;
    CStringDestroy(cs);
}
int compare_int(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}
int compareCString(const void *data1, const void *data2){
    CString *CS1 = (CString*)data1;
    CString *CS2 = (CString*)data2;
    return CStringCompare(CS1,CS2);
}
// 模拟的键值释放函数
void free_key(void *key) {
    if(key!=NULL)
        free(key);
}

// 模拟的值释放函数
void free_value(void *value) {
    free(value);
}

// 测试CMap初始化
static void test_CMapInit(void **state) {
    CMap map ;
    // 使用正确的整数比较函数 compare_ints
    int result = CMapInit(&map, sizeof(int), sizeof(int), compare_int, free_key, free_value,NULL,NULL); // free for int is not necessary
    assert_int_equal(result, 1);
}

// 测试CMap插入操作
static void test_CMapInsert(void **state) {
    CMap map;
    CMapInit(&map, sizeof(int), sizeof(int), compare_ints, NULL, NULL, NULL, NULL);

    int key = 1;
    int value = 100;

    int result = CMapInsert(&map, &key, &value);
    assert_int_equal(result, 1);

    // 再次插入相同 key —— Map 语义：应失败
    int value2 = 200;
    result = CMapInsert(&map, &key, &value2);
    assert_int_equal(result, 0);

    int *found_value = (int *)CMapFind(&map, &key);
    assert_non_null(found_value);
    assert_int_equal(*found_value, 100); // 原值不应被修改

    CMapDestroy(&map);
}


// 测试CMap查找操作
static void test_CMapFind(void **state) {
    CMap *map = malloc(sizeof (CMap));
    CMapInit(map, sizeof(int), sizeof(int), compare_ints, NULL, NULL,NULL,NULL);

    int key1 = 1, key2 = 2;
    int value1 = 100, value2 = 200;

    CMapInsert(map, &key1, &value1);
    CMapInsert(map, &key2, &value2);

    int *found_value1 = (int *)CMapFind(map, &key1);
    int *found_value2 = (int *)CMapFind(map, &key2);
    int *not_found = (int *)CMapFind(map, &(int){3});  // 查找不存在的键

    assert_non_null(found_value1);
    assert_int_equal(*found_value1, 100);

    assert_non_null(found_value2);
    assert_int_equal(*found_value2, 200);

    assert_null(not_found);  // 键3不应存在
    CMapDestroy(map); // 清理资源
}

// 测试CMap删除操作
static void test_CMapErase(void **state) {
    CMap map;
    CMapInit(&map, sizeof(int), sizeof(int), compare_ints, NULL, NULL,NULL,NULL);

    int key = 1;
    int value = 100;

    CMapInsert(&map, &key, &value);
    assert_non_null(CMapFind(&map, &key)); // 确保插入成功

    int result = CMapErase(&map, &key);
    assert_int_equal(result, 1);

    assert_null(CMapFind(&map, &key));  // 删除后应该找不到该键
    CMapDestroy(&map); // 清理资源
}

// 测试CMap迭代器
static void test_CMapIterator(void **state) {
    CMap map;
    CMapInit(&map, sizeof(int), sizeof(int), compare_ints, NULL, NULL,NULL,NULL);

    int key1 = 1, key2 = 2, key3 = 3;
    int value1 = 100, value2 = 200, value3 = 300;

    CMapInsert(&map, &key3, &value3); // 故意乱序插入以测试中序遍历
    CMapInsert(&map, &key1, &value1);
    CMapInsert(&map, &key2, &value2);

    CMapIterator it = CMapIteratorBegin(&map);
    CMapIterator end = CMapIteratorEnd(&map);
    assert_false(CMapIteratorEqual(&it,&end));
    assert_int_equal(*(int *)CMapIteratorKey(&it), 1);
    assert_int_equal(*(int *)CMapIteratorValue(&it), 100);

    CMapIteratorNext(&it);
    assert_false(CMapIteratorEqual(&it,&end));
    assert_int_equal(*(int *)CMapIteratorKey(&it), 2);
    assert_int_equal(*(int *)CMapIteratorValue(&it), 200);

    CMapIteratorNext(&it);
    assert_false(CMapIteratorEqual(&it,&end));
    assert_int_equal(*(int *)CMapIteratorKey(&it), 3);
    assert_int_equal(*(int *)CMapIteratorValue(&it), 300);

    CMapIteratorNext(&it); // 移到末尾
    assert_true(CMapIteratorEqual(&it,&end));

    CMapDestroy(&map); // 清理资源
}

// 测试CMapFree是否能正确释放资源 (通常结合内存泄漏检测工具)
static void test_CMapFree(void **state) {
    CMap map;
    CMapInit(&map, sizeof(int), sizeof(int), compare_ints, NULL, NULL,NULL,NULL);

    int key = 1;
    int value = 100;

    CMapInsert(&map, &key, &value);
    CMapDestroy(&map); // 期望这里能正确释放所有内存，如果使用 valgrind 等工具可以检测
    // 没有断言，因为主要依赖外部工具进行内存泄漏检测
}

// 字符串CMap的特定测试
static void test_CMapStringKeys(void **state) {
    CMap strMap;
    // 使用正确的字符串比较函数 compare_strings 和释放函数 free_string_ptr
    CMapInit(&strMap, sizeof(CString), sizeof(int), compareCString, free_string_ptr, NULL,CStringCreateFromCStringVoid,NULL);

    CString*key1 = CStringCreateFromCStr("key1");
    CString*key2 = CStringCreateFromCStr("key2");
    CString*key3 = CStringCreateFromCStr("key3");
    int val1 = 10, val2 = 20, val3 = 30;

    CMapInsert(&strMap, key1, &val1);
    CMapInsert(&strMap, key2, &val2);
    CMapInsert(&strMap, key3, &val3);

    int *found_val = (int *)CMapFind(&strMap, key2);
    assert_non_null(found_val);
    assert_int_equal(*found_val, 20);

    CString *key_not_exist = CStringCreateFromCStr("grape");
    assert_null(CMapFind(&strMap, key_not_exist));

    CMapErase(&strMap, key1);
    assert_null(CMapFind(&strMap, key1)); // 确保删除成功

    CMapDestroy(&strMap); // 期望能正确释放所有字符串键的内存
    CStringDestroy(key1);
    CStringDestroy(key2);
    CStringDestroy(key3);
}


int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_CMapInit),
            cmocka_unit_test(test_CMapInsert),
            cmocka_unit_test(test_CMapFind),
            cmocka_unit_test(test_CMapErase),
            cmocka_unit_test(test_CMapIterator),
            cmocka_unit_test(test_CMapFree), // 配合内存检测工具使用
            cmocka_unit_test(test_CMapStringKeys), // 新增字符串键测试
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}