//
// Created by yaohuayuan on 2024/9/23.
//
#include "LogManager.h"
LogManager *LogManagerInit(FileManager * fileManager,char*logfile){
    LogManager  *logManager = malloc(sizeof (LogManager));
    logManager->fileManager = fileManager;
    logManager->logFile = logfile;
    logManager->logPage=PageInit( logManager->fileManager->blockSize);
    logManager->LastSavedLSN = 0;
    logManager->latestLSN = 0;
    int logSize = FileManagerLength(logManager->fileManager,logfile);
    if(logSize == 0){
        logManager->currentBlockId = LogManagerAppendNewBlock(logManager);
    }else{
        BlockID_Init(&logManager->currentBlockId,logfile,logSize-1);
        FileManagerRead(logManager->fileManager,logManager->currentBlockId,logManager->logPage);
    }
    return logManager;
}
BlockID LogManagerAppendNewBlock(LogManager* logManager){
    BlockID  blockId = FileManagerAppend(logManager->fileManager,logManager->logFile);
    PageSetInt(logManager->logPage,0,logManager->fileManager->blockSize);
    FileManagerWrite(logManager->fileManager,blockId,logManager->logPage);
//    Page *p = PageInit(logManager->fileManager->blockSize);
//    FileManagerRead(logManager->fileManager,blockId,p);
//
//    ByteBufferData *out = ByteBufferDataInit();
//    PageGetInt(p,0,out);
//    printf("%d",*out->intData);
    return blockId;
}
int LogManagerAppend(LogManager *logManager, uint8_t *data, uint8_t *type,int size) {
    int boundary;
    ByteBufferData *byteBufferData = ByteBufferDataInit();
    PageGetInt(logManager->logPage, 0, byteBufferData);
    boundary = *byteBufferData->intData;
//    printf("Current boundary: %d\n", boundary); // 调试信息
    int byteNeeded = size + sizeof(int);
    if (boundary - byteNeeded <(int) sizeof (int)) { // the log record doesn't fit

        LogManagerFlush(logManager);
        logManager->currentBlockId = LogManagerAppendNewBlock(logManager);
        PageGetInt(logManager->logPage, 0, byteBufferData);
        boundary = *byteBufferData->intData;
//        printf("Boundary too small, flushing and appending new block\n");
//        BlockID  blockId;
//        BlockID_Init(&blockId,logManager->currentBlockId.fileName,0);
//        Page *page = PageInit(400);
//        FileManagerRead(logManager->fileManager,blockId,page);
//        ByteBufferData *out = ByteBufferDataInit();
//        PageGetInt(page, 0, out);
//        PageGetInt(logManager->logPage,380,out);

//        printf("New boundary after flush: %d\n", boundary); // 调试信息
    }
    int recPos = boundary - byteNeeded;
    PageSetBytes(logManager->logPage, recPos, data,type, size);
    PageSetInt(logManager->logPage, 0, recPos); // the new boundary
    logManager->latestLSN += 1;
//    printf("Updated boundary: %d\n", recPos); // 调试信息
    return logManager->latestLSN;
}
void LogManagerFlush(LogManager * logManager){
    FileManagerWrite(logManager->fileManager,logManager->currentBlockId,logManager->logPage);
    logManager->LastSavedLSN = logManager->latestLSN;

    Page *page = PageInit(400);
    BlockID blockId;
    BlockID_Init(&blockId,logManager->currentBlockId.fileName,0);
    FileManagerRead(logManager->fileManager,blockId,page);
    ByteBufferData *out = ByteBufferDataInit();
    PageGetInt(page, 0, out);
    return;
}
void LogManagerFlushLSN(LogManager * logManager,int lsn){
    if (lsn >=logManager->LastSavedLSN)
        LogManagerFlush(logManager);
}
LogIterator *LogManager2LogManager(LogManager *logManager){
    LogManagerFlush(logManager);
    LogIterator *logIterator = LogIteratorInit(logManager->fileManager,logManager->currentBlockId);
    return logIterator;
}
