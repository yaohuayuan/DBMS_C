//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_TRIE_H
#define DBMS_C_TRIE_H


#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define TRIE_CHARSET_SIZE 128  // 支持完整 ASCII 字符集

typedef struct Trie {
    bool isEnd;
    struct Trie* next[TRIE_CHARSET_SIZE];
} Trie;

/**
 * 初始化一个新的 Trie 树实例。
 *
 * @return 返回初始化后的 Trie 树指针。
 */
Trie* TrieInit();

/**
 * 向 Trie 树中插入一个字符串。
 *
 * @param root 指向 Trie 树根节点的指针。
 * @param s 要插入的字符串。
 */
void TrieInsert(Trie* root, const char* s);

/**
 * 在 Trie 树中搜索指定字符串。
 *
 * @param root 指向 Trie 树根节点的指针。
 * @param s 要搜索的字符串。
 * @return 如果找到返回 true，否则返回 false。
 */
bool TrieSearchIn(Trie* root, const char* s);

#endif // DBMS_C_TRIE_H
