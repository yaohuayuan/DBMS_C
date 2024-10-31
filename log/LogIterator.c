//
// Created by yaohuayuan on 2024/9/22.
//

#include "LogIterator.h"
LogIterator * LogIteratorInit(FileManager *fm,BlockID blockId){
    LogIterator * logIterator = malloc(sizeof(LogIterator));
    BlockID_Init(&logIterator->blockId,blockId.fileName,blockId.blockId);
    logIterator->fm=FileManagerInit(fm->dbDirectoryName,fm->blockSize);
    logIterator->page = PageInit(fm->blockSize);
    LogIteratorMoveToBlock(logIterator);
    return logIterator;
}
void LogIteratorMoveToBlock(LogIterator* logIterator){
    FileManagerRead(logIterator->fm,logIterator->blockId,logIterator->page);
    ByteBufferData *byteBufferData = ByteBufferDataInit();
    PageGetInt(logIterator->page,0,byteBufferData);
    logIterator->boundary=*byteBufferData->intData;
    logIterator->currentPos = logIterator->boundary;
}
bool LogIteratorHasNext(LogIterator *logIterator){
    return logIterator->currentPos<logIterator->fm->blockSize || logIterator->blockId.blockId>0;
}
//9.22 未写完
//9.23 写完
//9.29 修改版
ByteBufferData *LogIteratorNext(LogIterator *logIterator){
    if(logIterator->currentPos == logIterator->fm->blockSize){
        logIterator->blockId.blockId--;
        LogIteratorMoveToBlock(logIterator);
    }
    ByteBufferData* byteBufferData = PageGetBytes(logIterator->page,logIterator->currentPos);
    logIterator->currentPos += sizeof(int)+byteBufferData->bytesData->length;
    return byteBufferData;
}