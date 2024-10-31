//
// Created by yaohuayuan on 2024/9/23.
//

#ifndef DBMS_C_LOGMANAGER_H
#define DBMS_C_LOGMANAGER_H
#include "FileManager.h"
#include "LogIterator.h"
typedef struct {
    char * logFile;
    FileManager *fileManager;
    Page *logPage;
    BlockID currentBlockId;
    int latestLSN;
    int LastSavedLSN;
}LogManager;
LogManager *LogManagerInit(FileManager * fileManager,char*logfile);
BlockID LogManagerAppendNewBlock(LogManager* logManager);
void LogManagerFlush(LogManager *logManager);
LogIterator *LogManager2LogManager(LogManager *logManager);
void LogManagerFlushLSN(LogManager * logManager,int lsn);
int LogManagerAppend(LogManager *logManager, uint8_t *data, uint8_t *type,int size);
#endif //DBMS_C_LOGMANAGER_H