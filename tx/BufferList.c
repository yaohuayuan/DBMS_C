//
// Created by yaohuayuan on 2024/10/31.
//

#include "BufferList.h"
BufferTEMP *BufferTEMPInit(){
    BufferTEMP  *bufferTemp = malloc(sizeof (BufferTEMP));
    bufferTemp->buffer = NULL;
    return bufferTemp;
}
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
    BufferTEMP *bufferTemp= map_get(bufferList->buffers,BlockIDToString(blockId));
    Buffer*buffer = bufferTemp->buffer;
    return buffer;
}
void BufferListPin(BufferList *bufferList,BlockID blockId){
    Buffer  *buffer = BufferManagerPin(bufferList->bufferManager,blockId);
    BufferTEMP *bufferTemp = BufferTEMPInit();
    bufferTemp->buffer = buffer;
    map_set(bufferList->buffers,BlockIDToString(blockId),*bufferTemp);
    BlockIDNode *blockIdNode = bufferList->pin;
    while(blockIdNode->next!=NULL){
        blockIdNode=blockIdNode->next;
    }
    BlockIDNode *NewBlockIDNode = malloc(sizeof(BlockIDNode));
    BlockID_Init(&NewBlockIDNode->blockId,blockId.fileName,blockId.blockId);
    NewBlockIDNode->next = NULL;
    blockIdNode->next = NewBlockIDNode;
}
void BufferListUnPin(BufferList *bufferList,BlockID blockId){
    BufferTEMP *bufferTemp= map_get(bufferList->buffers,BlockIDToString(blockId));
    Buffer*buffer = bufferTemp->buffer;
    BufferManagerUnpin(bufferList->bufferManager,buffer);
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
void BufferListUnpinAll(BufferList *bufferList) {
    BlockIDNode *p = bufferList->pin->next, *temp;
    while (p) {
        BufferTEMP *bufferTemp= map_get(bufferList->buffers,BlockIDToString(p->blockId));
        Buffer*buffer = bufferTemp->buffer;
        BufferManagerUnpin(bufferList->bufferManager, buffer);
        p = p->next;
    }

    p = bufferList->pin;
    BlockIDNode *q = p->next;
    p->next = NULL;
    while (q) {
        temp = q;
        q = q->next;
        free(temp);
    }

    // 将所有键存储到一个列表中，稍后再删除
    const char *keys_to_remove[bufferList->buffers->base.nnodes];
    int index = 0;
    const char *key;
    map_iter_t iter = map_iter(&bufferList->buffers);
    while ((key = map_next(bufferList->buffers, &iter))) {
        printf("%s\n", key);
        keys_to_remove[index++] = key;
    }

    // 从缓冲区中删除所有键
    for (int i = 0; i < index; i++) {
        map_remove(bufferList->buffers, keys_to_remove[i]);
    }
}