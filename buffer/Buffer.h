//
// Created by yaohuayuan on 2024/10/8.
//

#ifndef DBMS_C_BUFFER_H
#define DBMS_C_BUFFER_H
#include "FileManager.h"
#include "LogManager.h"
#include "Page.h"
typedef struct Buffer{
    FileManager *fileManager;
    LogManager *logManager;
    Page *page;
    BlockID blockId;
    int pins;
    int txNum;
    int lsn;
}Buffer;
Buffer * BufferInit(FileManager *fileManager,LogManager *logManager);
void BufferSetModified(Buffer *buffer, int txNum,int lsn);
void BufferFlush(Buffer *buffer);
bool BufferIsPinned(Buffer *buffer);
void BufferPin(Buffer *buffer);
void BufferUnPin(Buffer *buffer);
void BufferAssignToBlock(Buffer *buffer,BlockID blockId);
#endif //DBMS_C_BUFFER_H
