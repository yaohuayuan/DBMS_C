//
// Created by yaohuayuan on 2024/11/19.
//

#include "Trie.h"
Trie *TrieCreatNode(){
    Trie *trie = malloc(sizeof(Trie));
    trie->isEnd = false;
    for(int i=0;i<26;i++){
        trie->next[i] = NULL;
    }
    return trie;
}
Trie *TrieInit(){
    return TrieCreatNode();
}
void TrieInsert(Trie*root,const char *s){
    Trie* current = root;
    for (int i = 0;s[i] != '\0'; i++) {
        int index = s[i] - 'a'; // 假设处理的是小写字母
        if (current->next[index] == NULL) {
            current->next[index] = TrieCreatNode();
        }
        current = current->next[index];
    }
    current->isEnd = true; // 设置为单词结束标志
}
bool TrieSearchIn(Trie*root,const char *s){
    Trie * current = root;
    for (int i = 0; s[i] != '\0'; i++) {
        int index = s[i] - 'a';
        if (current->next[index] == NULL) {
            return false; // 不存在此前缀
        }
        current = current->next[index];
    }
    return current->isEnd; // 返回是否找到了完整单词
}