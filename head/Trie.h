//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_TRIE_H
#define DBMS_C_TRIE_H
#include "stdbool.h"
#include "malloc.h"
#include "stdio.h"
typedef struct Trie{
    bool isEnd;
    struct Trie *next[26];
}Trie;
Trie *TrieInit();
void TrieInsert(Trie*head,const char *s);
bool TrieSearchIn(Trie*head,const char *s);
#endif //DBMS_C_TRIE_H
