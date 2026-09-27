#include "Trie.h"

Trie *TrieCreateNode() {
    Trie *node = malloc(sizeof(Trie));
    if (!node) {
        return NULL;
    }

    node->isEnd = false;
    for (int i = 0; i < TRIE_CHARSET_SIZE; i++) {
        node->next[i] = NULL;
    }
    return node;
}

Trie *TrieInit() {
    return TrieCreateNode();
}

void TrieInsert(Trie *root, const char *s) {
    if (!root || !s) {
        return;
    }

    Trie *current = root;
    for (int i = 0; s[i] != '\0'; i++) {
        unsigned char index = (unsigned char)s[i]; // 支持全 ASCII 范围
        if (index >= TRIE_CHARSET_SIZE) {
            return;
        }

        if (current->next[index] == NULL) {
            current->next[index] = TrieCreateNode();
            if (!current->next[index]) {
                return;
            }
        }
        current = current->next[index];
    }
    current->isEnd = true;
}

bool TrieSearchIn(Trie *root, const char *s) {
    if (!root || !s) {
        return false;
    }

    Trie *current = root;
    for (int i = 0; s[i] != '\0'; i++) {
        unsigned char index = (unsigned char)s[i];
        if (index >= TRIE_CHARSET_SIZE || !current || current->next[index] == NULL) {
            return false;
        }
        current = current->next[index];
    }
    return current && current->isEnd;
}
