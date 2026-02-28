//
// Created by Lenovo on 2026/1/14.
//

#include "LRUCore.h"
#include"CVector.h"
#include"Buffer.h"
#define HASH_TABLE_SIZE 257

LRUNode* LRUNodeInit(int key,int data){
    LRUNode* node = (LRUNode*)malloc(sizeof(LRUNode));
    node->key = key;
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    return node;
}
LRUList *LRUListInit(){
    LRUList*lru_list = (LRUList*)malloc(sizeof(LRUList));
    lru_list->head = LRUNodeInit(-1,-1);
    lru_list->tail = LRUNodeInit(-1,-1);
    lru_list->head->next = lru_list->tail;
    lru_list->tail->prev = lru_list->head;
    return lru_list;
}
void LRUListInsertToHead(LRUList *lru_list, LRUNode *node){
    LRUNode*head = lru_list->head;
    LRUNode*head_next = head->next;
    node->prev = head;
    node->next = head_next;
    head_next->prev = node;
    head->next = node;

}
void LRUListRemoveNode(LRUNode *node){
    if (node->next){
        node->next->prev = node->prev;
    }
    node->prev->next = node->next;
}
void LRUListMoveToHead(LRUList *lru_list,LRUNode *node){
    LRUListRemoveNode(node);
    LRUListInsertToHead(lru_list,node);
}
LRUNode* LRUListRemoveTail(LRUList *lru_list){
    if (lru_list->tail->prev == lru_list->head)
        return NULL;

    LRUNode* node = lru_list->tail->prev;
    LRUListRemoveNode(node);
    return node;
}


HashMap* HashMapInit(int size){
    HashMap* hash_map = (HashMap*)malloc(sizeof(HashMap));
    hash_map->table = CVectorInit(sizeof(LRUNode*),NULL,NULL,NULL);
    for (int i=0;i<size;i++){
        LRUNode* node = NULL;
        CVectorPushBack(hash_map->table,&node);
    }
    hash_map->size = size;
    return hash_map;
}
LRUNode*HashMapGet(HashMap *hash_map,int key){
    int pos = key%hash_map->size;
    LRUNode*head =*(LRUNode**)CVectorAt(hash_map->table,pos);
    while(head){
        if(head->key == key){
            return head;
        }
        head = head->hNext;
    }
    return NULL;
}
void HashMapPut(HashMap* hash_map, LRUNode* node){
    const int pos = node->key % hash_map->size;
    LRUNode** bucket = (LRUNode**)CVectorAt(hash_map->table, pos);

    node->hNext = *bucket;
    *bucket = node;
}

void HashMapErase(HashMap* hash_map, LRUNode* node){
    const int pos = node->key % hash_map->size;
    LRUNode** bucket = (LRUNode**)CVectorAt(hash_map->table, pos);

    LRUNode* cur = *bucket;
    LRUNode* prev = NULL;

    while (cur) {
        if (cur == node) {
            if (prev)
                prev->hNext = cur->hNext;
            else
                *bucket = cur->hNext;
            return;
        }
        prev = cur;
        cur = cur->hNext;
    }
}


LRUCore* LRUCoreCreate() {
    LRUCore* core = malloc(sizeof(LRUCore));
    core->list = LRUListInit();
    core->map  = HashMapInit(HASH_TABLE_SIZE);
    return core;
}

void LRUCoreAccess(LRUCore* core, int key) {
    LRUNode* node = HashMapGet(core->map, key);
    if (node) {
        LRUListMoveToHead(core->list, node);
        return;
    }

    node = LRUNodeInit(key, 0);
    LRUListInsertToHead(core->list, node);
    HashMapPut(core->map, node);
}

int LRUCoreVictim(LRUCore* core) {
    LRUNode* node = core->list->tail->prev;
    if (node == core->list->head)
        return -1;
    return node->key;
}

void LRUCoreRemove(LRUCore* core, int key) {
    LRUNode* node = HashMapGet(core->map, key);
    if (!node) return;

    LRUListRemoveNode(node);
    HashMapErase(core->map, node);
    free(node);
}
void LRUCoreDestroy(LRUCore* core) {
    if (!core) return;
    LRUNode* cur = core->list->head;
    while (cur) {
        LRUNode* next = cur->next;
        free(cur);
        cur = next;
    }
    free(core->list);
    CVectorDestroy(core->map->table);
    free(core->map);

    free(core);
}
