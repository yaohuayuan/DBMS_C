//
// Created by Lenovo on 2025/6/24.
//
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "RBT.h"  // 包含红黑树头文件
#include "malloc.h"

// 模拟的键比较函数
int compare_int(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// 模拟的键值释放函数
void free_key(void *key) {
    free(key);
}

// 模拟的值释放函数
void free_value(void *value) {
    free(value);
}

// 测试红黑树的初始化
static void test_rbt_init(void **state) {
    RBTree tree;
    int result = RBTreeInit(&tree, compare_int, free_key, free_value);
    assert_int_equal(result, 1);  // 初始化成功应该返回1
    assert_non_null(tree.nil);    // nil 节点应不为 NULL
    assert_non_null(tree.root);   // root 节点应不为 NULL
}

// 测试插入操作
static void test_rbt_insert(void **state) {
    RBTree tree;
    RBTreeInit(&tree, compare_int, free_key, free_value);

    // 插入键值对
    int *key = (int*)malloc(sizeof(int));
    int *value = (int*)malloc(sizeof(int));
    *key = 10;
    *value = 100;

    assert_int_equal(RBTreeInsert(&tree, key, value), 1); // 插入成功返回1

    // 查找插入的键
    RBNode *node = RBTreeSearch(&tree, tree.root, key);
    assert_non_null(node);        // 找到节点
    assert_int_equal(*(int*)node->key, 10); // 键值应该为 10
    assert_int_equal(*(int*)node->data, 100); // 数据值应该为 100
}

// 测试删除操作
static void test_rbt_delete(void **state) {
    RBTree tree;
    RBTreeInit(&tree, compare_int, free_key, free_value);

    int *key = (int*)malloc(sizeof(int));
    int *value = (int*)malloc(sizeof(int));
    *key = 10;
    *value = 100;

    RBTreeInsert(&tree, key, value); // 插入一个键值对

    // 删除操作
    assert_int_equal(RBTreeDelete(&tree, key), 1); // 删除成功返回 1

    // 再次查找删除的键，应该找不到
    RBNode *node = RBTreeSearch(&tree, tree.root, key);
    assert_ptr_equal(node, tree.nil);  // 如果删除成功，返回 nil 节点
}

// 测试查找操作
static void test_rbt_search(void **state) {
    RBTree tree;
    RBTreeInit(&tree, compare_int, free_key, free_value);

    int *key1 = (int*)malloc(sizeof(int));
    int *value1 = (int*)malloc(sizeof(int));
    *key1 = 10;
    *value1 = 100;
    RBTreeInsert(&tree, key1, value1); // 插入一个键值对

    int *key2 = (int*)malloc(sizeof(int));
    *key2 = 20;
    int *value2 = (int*)malloc(sizeof(int));
    *value2 = 200;
    RBTreeInsert(&tree, key2, value2); // 插入第二个键值对

    // 查找第一个插入的键
    RBNode *node1 = RBTreeSearch(&tree, tree.root, key1);
    assert_non_null(node1);
    assert_int_equal(*(int*)node1->key, 10); // 键值应该为 10
    assert_int_equal(*(int*)node1->data, 100); // 数据值应该为 100

    // 查找第二个插入的键
    RBNode *node2 = RBTreeSearch(&tree, tree.root, key2);
    assert_non_null(node2);
    assert_int_equal(*(int*)node2->key, 20); // 键值应该为 20
    assert_int_equal(*(int*)node2->data, 200); // 数据值应该为 200
}

// 测试内存释放
static void test_rbt_free(void **state) {
    RBTree tree;
    RBTreeInit(&tree, compare_int, free_key, free_value);

    // 插入键值对
    int *key = (int*)malloc(sizeof(int));
    int *value = (int*)malloc(sizeof(int));
    *key = 10;
    *value = 100;
    RBTreeInsert(&tree, key, value);

    // 释放红黑树资源
    RBTreeFree(&tree);
    // 不进行断言，因为释放后树应当不再有效
}

// 汇总所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_rbt_init),
            cmocka_unit_test(test_rbt_insert),
            cmocka_unit_test(test_rbt_delete),
            cmocka_unit_test(test_rbt_search),
            cmocka_unit_test(test_rbt_free),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
