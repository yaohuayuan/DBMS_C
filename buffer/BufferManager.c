//
// Created by Lenovo on 2025/7/16.
//

#include "BufferManager.h"
BufferManager *  BufferManagerInit(FileManager *fileManager,LogManager *logManager,int numBuffs){
    BufferManager *bufferManager = malloc(sizeof(BufferManager));
    bufferManager->bufferSize = numBuffs;
    bufferManager->bufferPool = malloc(sizeof (Buffer)*numBuffs);
    for(int i=0;i<numBuffs;i++){
        bufferManager->bufferPool[i] = BufferInit(fileManager,logManager);
    }
    bufferManager->numAvailable = numBuffs;
    return bufferManager;
}
void BufferManagerFlushAll(BufferManager *bufferManager,int tx){
    for(int i=0;i<bufferManager->bufferSize;i++){
        if(bufferManager->bufferPool[i]->txNum == tx){
            BufferFlush(bufferManager->bufferPool[i]);
        }
    }
}
void BufferManagerUnpin(BufferManager *bufferManager,Buffer *buffer){
    BufferUnPin(buffer);
    if(!BufferIsPinned(buffer)){
        bufferManager->numAvailable++;
    }
}
Buffer* BufferManagerFindExistingBuffer(BufferManager *bufferManager,BlockID *blockId){
    for(int i=0;i<bufferManager->bufferSize;i++){
        Buffer *buffer = bufferManager->bufferPool[i];
        if(buffer->blockId->BlockID!=-1&&buffer->blockId->fileName!=NULL&&BlockIDEqual(blockId,buffer->blockId))
            return buffer;
    }
    return NULL;
}
Buffer* BufferManagerChooseUnPinnedBuffer(BufferManager* bufferManager){
    Buffer *lru = NULL;
    time_t oldest = time(NULL) + 1;
    for (int i = 0; i < bufferManager->bufferSize; i++) {
        Buffer *buf = bufferManager->bufferPool[i];
        if (!BufferIsPinned(buf) && buf->lastUsed < oldest) {
            oldest = buf->lastUsed;
            lru = buf;
        }
    }
    return lru;
}
Buffer* BufferManagerTryToPin(BufferManager *bufferManager, BlockID *blockId){
    Buffer *buffer = BufferManagerFindExistingBuffer(bufferManager,blockId);
    if(buffer == NULL){
        buffer = BufferManagerChooseUnPinnedBuffer(bufferManager);
        if(buffer == NULL){
            return NULL;
        }
        BufferAssignToBlock(buffer,blockId);
    }
    if(!BufferIsPinned(buffer)){
        bufferManager->numAvailable--;
    }
    BufferPin(buffer);
    return buffer;
}
bool BufferManagerWaitTooLong(long startTime){
    long nowTime = time(NULL);
    return nowTime-startTime>MAX_TIME;
}
Buffer *  BufferManagerPin(BufferManager *bufferManager,BlockID *blockId){
    Buffer *buffer = BufferManagerTryToPin(bufferManager,blockId);
    long startTime = time(NULL);
    while(buffer == NULL&& !BufferManagerWaitTooLong(startTime)){
        buffer = BufferManagerTryToPin(bufferManager,blockId);
    }
    return buffer;
}