//
// Created by yaohuayuan on 2024/10/8.
//

#ifndef DBMS_C_BUFFERMANAGER_H
#define DBMS_C_BUFFERMANAGER_H
#include "Buffer.h"
#include "time.h"
typedef struct BufferManager{
    Buffer **bufferPool;
    int bufferSize;
    int numAvailable;
}BufferManager;
#define MAX_TIME  10
BufferManager *  BufferManagerInit(FileManager *fileManager,LogManager *logManager,int numBuffs);
void BufferManagerFlushAll(BufferManager *bufferManager,int tx);
void BufferManagerUnpin(BufferManager *bufferManager,Buffer *buffer);
Buffer* BufferManagerFindExistingBuffer(BufferManager *bufferManager,BlockID blockId);
Buffer* BufferManagerChooseUnPinnedBuffer(BufferManager* bufferManager);
Buffer* BufferManagerTryToPin(BufferManager *bufferManager, BlockID blockId);
bool BufferManagerWaitTooLong(long startTime);
Buffer *  BufferManagerPin(BufferManager *bufferManager,BlockID blockId);
#endif //DBMS_C_BUFFERMANAGER_H
