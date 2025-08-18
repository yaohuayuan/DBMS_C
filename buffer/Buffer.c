//
// Created by Lenovo on 2025/7/16.
//

#include <time.h>
#include "Buffer.h"
Buffer * BufferInit(FileManager *fileManager,LogManager *logManager){
    Buffer *buffer = malloc(sizeof (Buffer));
    buffer->pins = 0;
    buffer->lsn = -1;
    buffer->txNum = -1;
    buffer->fileManager = fileManager;
    buffer->logManager = logManager;
    buffer->page = PageInit(fileManager->blockSize);
    buffer->blockId = BlockIDInit(CStringCreateFromCStr(""),-1);
    buffer->lastUsed = time(NULL);
    return buffer;
}
void BufferSetModified(Buffer *buffer, int txNum,int lsn){
    buffer->txNum = txNum;
    if(lsn>=0){
        buffer->lsn = lsn;
    }
}
void BufferFlush(Buffer *buffer){
    if(buffer->txNum>=0){
        LogManagerFlushLSN(buffer->logManager,buffer->lsn);
        FileManagerWrite(buffer->fileManager,buffer->blockId,buffer->page);
        buffer->txNum = -1;
    }
}
bool BufferIsPinned(Buffer *buffer){
    return buffer->pins>0;
}
void BufferPin(Buffer *buffer){
    buffer->pins++;
    buffer->lastUsed = time(NULL);
}
void BufferUnPin(Buffer *buffer){
    buffer->pins--;
    buffer->lastUsed = time(NULL);
}
void BufferAssignToBlock(Buffer *buffer,BlockID *blockId){
    BufferFlush(buffer);
    buffer->blockId=BlockIDInit(blockId->fileName,blockId->BlockID);
    FileManagerRead(buffer->fileManager,blockId,buffer->page);
    buffer->pins = 0;
}