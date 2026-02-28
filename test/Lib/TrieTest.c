#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdint.h>
#include "Trie.h"

// 测试 TrieInit
static void test_trie_init(void **state) {
    (void)state;

    Trie *root = TrieInit();

    assert_non_null(root);
    assert_false(root->isEnd);
    
    // 检查所有子节点初始为空
    for (int i = 0; i < TRIE_CHARSET_SIZE; i++) {
        assert_null(root->next[i]);
    }
    
    // 目前没有提供释放函数，所以不释放
}

// 测试 TrieInsert 和 TrieSearchIn
static void test_trie_insert_and_search(void **state) {
    (void)state;

    Trie *root = TrieInit();
    
    // 插入一些字符串
    TrieInsert(root, "hello");
    TrieInsert(root, "world");
    TrieInsert(root, "test");
    TrieInsert(root, "trie");
    
    // 测试存在的字符串
    assert_true(TrieSearchIn(root, "hello"));
    assert_true(TrieSearchIn(root, "world"));
    assert_true(TrieSearchIn(root, "test"));
    assert_true(TrieSearchIn(root, "trie"));
    
    // 测试不存在的字符串
    assert_false(TrieSearchIn(root, "hell"));
    assert_false(TrieSearchIn(root, "helloworld"));
    assert_false(TrieSearchIn(root, "tes"));
    assert_false(TrieSearchIn(root, "triee"));
    assert_false(TrieSearchIn(root, "notexist"));
    
    // 测试空字符串
    assert_false(TrieSearchIn(root, ""));
    
    // 插入空字符串
    TrieInsert(root, "");
    assert_true(TrieSearchIn(root, ""));
    
    // 目前没有提供释放函数，所以不释放
}

// 测试 Trie 树的前缀匹配特性
static void test_trie_prefix(void **state) {
    (void)state;

    Trie *root = TrieInit();
    
    // 插入具有相同前缀的字符串
    TrieInsert(root, "prefix");
    TrieInsert(root, "pref");
    TrieInsert(root, "pre");
    TrieInsert(root, "prefixes");
    
    // 测试前缀和完整字符串
    assert_true(TrieSearchIn(root, "pre"));
    assert_true(TrieSearchIn(root, "pref"));
    assert_true(TrieSearchIn(root, "prefix"));
    assert_true(TrieSearchIn(root, "prefixes"));
    
    // 测试不存在的前缀
    assert_false(TrieSearchIn(root, "prefixe"));
    assert_false(TrieSearchIn(root, "pr"));
    assert_false(TrieSearchIn(root, "p"));
    
    // 目前没有提供释放函数，所以不释放
}

// 测试 Trie 树的重复插入
static void test_trie_duplicate_insert(void **state) {
    (void)state;

    Trie *root = TrieInit();
    
    // 多次插入相同的字符串
    TrieInsert(root, "duplicate");
    TrieInsert(root, "duplicate");
    TrieInsert(root, "duplicate");
    
    // 仍然应该只找到一次
    assert_true(TrieSearchIn(root, "duplicate"));
    
    // 目前没有提供释放函数，所以不释放
}

// 主函数：注册所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_trie_init),
            cmocka_unit_test(test_trie_insert_and_search),
            cmocka_unit_test(test_trie_prefix),
            cmocka_unit_test(test_trie_duplicate_insert),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}