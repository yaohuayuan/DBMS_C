//
// Created by yaohuayuan on 2024/10/8.
//

#include "Buffer.h"
Buffer * BufferInit(FileManager *fileManager,LogManager *logManager){
    Buffer *buffer = malloc(sizeof (Buffer));
    buffer->pins = 0;
    buffer->lsn = -1;
    buffer->txNum = -1;
    buffer->fileManager = FileManagerInit(fileManager->dbDirectoryName,fileManager->blockSize);
    buffer->logManager = LogManagerInit(logManager->fileManager,logManager->logFile);
    buffer->page = PageInit(fileManager->blockSize);
    BlockID_Init(&buffer->blockId,NULL,-1);
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
}
void BufferUnPin(Buffer *buffer){
    buffer->pins--;
}
void BufferAssignToBlock(Buffer *buffer,BlockID blockId){
    BufferFlush(buffer);
    BlockID_Init(&buffer->blockId,blockId.fileName,blockId.blockId);
    FileManagerRead(buffer->fileManager,blockId,buffer->page);
    buffer->pins = 0;
}