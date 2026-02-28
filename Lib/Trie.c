//
// Created by yaohuayuan on 2024/11/19.
//

#include "Trie.h"

Trie* TrieCreateNode() {
    Trie* node = malloc(sizeof(Trie));
    node->isEnd = false;
    for (int i = 0; i < TRIE_CHARSET_SIZE; i++) {
        node->next[i] = NULL;
    }
    return node;
}

Trie* TrieInit() {
    return TrieCreateNode();
}

void TrieInsert(Trie* root, const char* s) {
    Trie* current = root;
    for (int i = 0; s[i] != '\0'; i++) {
        unsigned char index = (unsigned char)s[i];  // 支持全 ASCII 范围
        if (current->next[index] == NULL) {
            current->next[index] = TrieCreateNode();
        }
        current = current->next[index];
    }
    current->isEnd = true;
}

bool TrieSearchIn(Trie* root, const char* s) {
    Trie* current = root;
    for (int i = 0; s[i] != '\0'; i++) {
        unsigned char index = (unsigned char)s[i];
        if (index >= TRIE_CHARSET_SIZE || current->next[index] == NULL) {
            return false;
        }
        current = current->next[index];
    }
    return current->isEnd;
}
