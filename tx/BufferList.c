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

}