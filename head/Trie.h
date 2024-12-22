//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_TRIE_H
#define DBMS_C_TRIE_H

#include <stdbool.h>
#include <malloc.h>
#include <stdio.h>

/**
 * @brief Trie 结构体定义，用于实现字典树（前缀树）。
 */
typedef struct Trie {
    bool isEnd;                  ///< 标记是否为单词结尾
    struct Trie *next[26];       ///< 指向子节点的指针数组，假设只包含小写字母 a-z
} Trie;

/**
 * @brief 初始化一个新的 Trie 节点。
 *
 * @return 返回初始化后的 Trie 节点指针。
 */
Trie* TrieInit();

/**
 * @brief 向 Trie 中插入一个字符串。
 *
 * @param head 指向 Trie 的根节点指针。
 * @param s 要插入的字符串。
 */
void TrieInsert(Trie *head, const char *s);

/**
 * @brief 在 Trie 中查找一个字符串是否存在。
 *
 * @param head 指向 Trie 的根节点指针。
 * @param s 要查找的字符串。
 * @return 如果找到该字符串，则返回 true；否则返回 false。
 */
bool TrieSearchIn(Trie *head, const char *s);

#endif //DBMS_C_TRIE_H