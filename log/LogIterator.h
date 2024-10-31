//
// Created by yaohuayuan on 2024/9/22.
//

#ifndef DBMS_C_LOGITERATOR_H
#define DBMS_C_LOGITERATOR_H
#include "FileManager.h"
typedef struct {
    FileManager *fm;
    BlockID blockId;
    Page *page;
    int currentPos;
    int boundary;
}LogIterator;
LogIterator * LogIteratorInit(FileManager *fm,BlockID blockId);
void LogIteratorMoveToBlock(LogIterator* logIterator);
bool LogIteratorHasNext(LogIterator *logIterator);
ByteBufferData * LogIteratorNext(LogIterator *logIterator);
#endif //DBMS_C_LOGITERATOR_H
