//
// Created by yaohuayuan on 2024/10/31.
//

#ifndef DBMS_C_BUFFERLIST_H
#define DBMS_C_BUFFERLIST_H
#include "CMap.h"
#include "Buffer.h"
#include "BufferManager.h"

#include "map.h"
/**
 * @brief BlockIDNode 结构体，用于表示BlockID的链表节点。
 */
typedef struct BlockIDNode{
    BlockID *blockId;  ///< 指向BlockID的指针
    struct BlockIDNode* next;  ///< 指向下一个BlockIDNode的指针
}BlockIDNode;

/**
 * @brief BufferTEMP 结构体，用于临时存储Buffer指针。
 */
typedef struct BufferTEMP{
    Buffer *buffer;  ///< 指向Buffer的指针
}BufferTEMP;

/**
 * @brief 定义Buffer映射类型，用于将BlockID映射到Buffer。
 */
typedef  map_t(BufferTEMP) map_Buffer_t;

/**
 * @brief BufferList 结构体，用于管理事务使用的缓冲区列表。
 */
typedef struct BufferList{
    map_Buffer_t *buffers;  ///< Buffer映射表
    BlockIDNode *pin;  ///< 固定的BlockID链表
    BufferManager *bufferManager;  ///< 缓冲管理器
}BufferList;

/**
 * @brief 初始化一个新的BufferList实例。
 *
 * @param bufferManager 指向BufferManager的指针。
 * @return 返回初始化后的BufferList指针。
 */
BufferList *BufferListInit(BufferManager *bufferManager);

/**
 * @brief 获取指定BlockID对应的Buffer。
 *
 * @param bufferList 指向BufferList的指针。
 * @param blockId 指向BlockID的指针。
 * @return 返回对应的Buffer指针。
 */
Buffer* BufferListGetBuffer(BufferList *bufferList,BlockID* blockId);

/**
 * @brief 固定指定的BlockID对应的Buffer。
 *
 * @param bufferList 指向BufferList的指针。
 * @param blockId 指向BlockID的指针。
 */
void BufferListPin(BufferList *bufferList,BlockID* blockId);

/**
 * @brief 取消固定指定的BlockID对应的Buffer。
 *
 * @param bufferList 指向BufferList的指针。
 * @param blockId 指向BlockID的指针。
 */
void BufferListUnPin(BufferList *bufferList,BlockID* blockId);

/**
 * @brief 取消固定所有的Buffer。
 *
 * @param bufferList 指向BufferList的指针。
 */
void BufferListUnpinAll(BufferList *bufferList);
#endif //DBMS_C_BUFFERLIST_H
