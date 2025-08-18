//
// Created by Lenovo on 2025/7/31.
//

#ifndef NEWDBMS_DBMS_H
#define NEWDBMS_DBMS_H
#include "FileManager.h"
#include "BufferManager.h"
#include "LogManager.h"
#include "Transaction.h"
#include "MetadataManager.h"
#include "Planner.h"
#include "windows.h"
#define SIMPLE_DB_INIT_VAL (-1)
#define BLOCK_SIZE 4096
#define BUFFER_SIZE 8
#define LOG_FILE "SimpleDB.log"
typedef struct SimpleDB{
    FileManager *fileManager;
    BufferManager *bufferManager;
    LogManager *logManager;
    MetadataMgr *metadataMgr;
    Planner *planer;
//    HANDLE bgThread;   // 后台刷盘线程
//    HANDLE bgEvent;    // 事件对象，用来唤醒
}SimpleDB;
SimpleDB *SimpleDBInit(char *dirname,int blocksize,int buffersize);
Transaction *SimpleDataNewTX(SimpleDB*simpleDb);
Transaction *SimpleDataNewTXName(SimpleDB*simpleDb,char* *name);
#endif //NEWDBMS_DBMS_H
