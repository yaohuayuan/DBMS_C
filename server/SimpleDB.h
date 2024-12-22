//
// Created by yaohuayuan on 2024/11/21.
//

#ifndef DBMS_C_SIMPLEDB_H

#define DBMS_C_SIMPLEDB_H
#include "FileManager.h"
#include "BufferManager.h"
#include "LogManager.h"
#include "MetadataManager.h"
#include "Planner.h"
#include "TransactionManager.h"
#define SIMPLE_DB_INIT_VAL (-1)
#define BLOCK_SIZE 400
#define BUFFER_SIZE 8
#define LOG_FILE "SimpleDB.log"
typedef struct SimpleDB{
    FileManager *fileManager;
    BufferManager *bufferManager;
    LogManager *logManager;
    MetadataMgr *metadataMgr;
//    TransactionManager *transactionManager;
    Planner *planer;
}SimpleDB;
SimpleDB *SimpleDBInit(char *dirname,int blocksize,int buffersize);
Transaction *SimpleDataNewTX(SimpleDB*simpleDb);
Transaction *SimpleDataNewTXName(SimpleDB*simpleDb,char *name);
#endif //DBMS_C_SIMPLEDB_H
