//
// Created by yaohuayuan on 2024/10/31.
//

#include "BufferList.h"
BufferList *BufferListInit(BufferManager *bufferManager){
    BufferList*bufferList = malloc(sizeof(BufferList));
    bufferList->bufferManager = bufferManager;
    bufferList->buffers  = malloc(sizeof(map_Buffer_t ));
    map_init(bufferList->buffers);
    bufferList->pin = malloc(sizeof(BlockIDNode));
    bufferList->pin->next = NULL;
    return bufferList;
}
Buffer* BufferListGetBuffer(BufferList *bufferList,BlockID blockId){
    Buffer*buffer = map_get(bufferList->buffers,BlockIDToString(blockId));
    return buffer;
}
void BufferListPin(BufferList *bufferList,BlockID blockId){
    Buffer  *buffer = BufferManagerPin(bufferList->bufferManager,blockId);
    map_set(bufferList->buffers,BlockIDToString(blockId),*buffer);
    BlockIDNode *blockIdNode = bufferList->pin;
    while(blockIdNode->next!=NULL){
        blockIdNode=blockIdNode->next;
    }
    BlockIDNode *NewBlockIDNode = malloc(sizeof(BlockIDNode));
    BlockID_Init(&NewBlockIDNode->blockId,blockId.fileName,blockId.blockId);
    blockIdNode->next = NewBlockIDNode;
}
void BufferListUnPin(BufferList *bufferList,BlockID blockId){
    Buffer*buffer = map_get(bufferList->buffers,BlockIDToString(blockId));
    BlockIDNode *p = bufferList->pin->next;
    BlockIDNode *q = bufferList->pin;
    while(p){
        if(BlockIdEqual(p->blockId,blockId)){
            q->next = p->next;
            break;
        }
        q = p;
        p = p->next;
    }
    free(p);
    p = bufferList->pin->next;
    bool flag = 0;
    while (p){
        if(BlockIdEqual(p->blockId,blockId)){
            flag = 1;
            break;
        }
        p=p->next;
    }
    if(flag){
        map_remove(bufferList->buffers, BlockIDToString(blockId));
    }
}
void BufferListUnpinAll(BufferList *bufferList){
    BlockIDNode *p = bufferList->pin->next,*temp;
    while (p){
        Buffer * buffer = map_get(bufferList->buffers,BlockIDToString(p->blockId));
        BufferManagerUnpin(bufferList->bufferManager,buffer);
        p = p->next;
    }
    p = bufferList->pin;
    BlockIDNode *q = p->next;
    p->next = NULL;
    while(q){
        temp=q;
        q=q->next;
        free(temp);
    }
    const char *key;
    map_iter_t iter = map_iter(&m);
    while ((key = map_next(bufferList->buffers, &iter))) {
        map_remove(bufferList->buffers,key);
    }
}