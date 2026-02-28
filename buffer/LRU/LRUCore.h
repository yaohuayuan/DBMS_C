//
// Created by Lenovo on 2026/1/14.
//

#ifndef NEWDBMS_LRUCORE_H
#define NEWDBMS_LRUCORE_H
#include "CVector.h"
typedef struct LRUNode{
    struct LRUNode* next;
    struct LRUNode* prev;
    struct LRUNode*hNext;
    int key,data;
}LRUNode;
typedef struct LRUList{
    LRUNode* head;
    LRUNode* tail;
}LRUList;
typedef struct HashMap{
    CVector* table;
    int size;
}HashMap;
typedef struct LRUCore{
    LRUList*list;
    HashMap* map;
}LRUCore;
/**
 * 创建一个新的 LRUCore 实例。
 *
 * @return 返回初始化后的 LRUCore 指针。
 */
LRUCore* LRUCoreCreate() ;

/**
 * 记录对指定 key 的访问。
 *
 * @param core 指向 LRUCore 的指针。
 * @param key 被访问的 key。
 */
void LRUCoreAccess(LRUCore* core, int key);

/**
 * 选择一个要淘汰的 key。
 *
 * @param core 指向 LRUCore 的指针。
 * @return 返回要淘汰的 key。
 */
int LRUCoreVictim(LRUCore* core);

/**
 * 从 LRUCore 中移除指定 key。
 *
 * @param core 指向 LRUCore 的指针。
 * @param key 要移除的 key。
 */
void LRUCoreRemove(LRUCore* core, int key);

/**
 * 销毁 LRUCore 实例并释放资源。
 *
 * @param core 指向要销毁的 LRUCore 的指针。
 */
void LRUCoreDestroy(LRUCore* core);
#endif //NEWDBMS_LRUCORE_H