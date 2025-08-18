//
// Created by yaohuayuan on 2024/10/31.
//

#ifndef DBMS_C_BUFFERLIST_H
#define DBMS_C_BUFFERLIST_H
#include "CMap.h"
#include "Buffer.h"
#include "BufferManager.h"

#include "map.h"
typedef struct BlockIDNode{
    BlockID *blockId;
    struct BlockIDNode* next;
}BlockIDNode;
typedef struct BufferTEMP{
    Buffer *buffer;
}BufferTEMP;
typedef  map_t(BufferTEMP) map_Buffer_t;
typedef struct BufferList{
    map_Buffer_t *buffers;
    BlockIDNode *pin;
    BufferManager *bufferManager;
}BufferList;
BufferList *BufferListInit(BufferManager *bufferManager);
Buffer* BufferListGetBuffer(BufferList *bufferList,BlockID* blockId);
void BufferListPin(BufferList *bufferList,BlockID* blockId);
void BufferListUnPin(BufferList *bufferList,BlockID* blockId);
void BufferListUnpinAll(BufferList *bufferList);
#endif //DBMS_C_BUFFERLIST_H
