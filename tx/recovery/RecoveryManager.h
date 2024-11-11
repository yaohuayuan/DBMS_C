//
// Created by yaohuayuan on 2024/11/10.
//

#ifndef DBMS_C_RECOVERYMANAGER_H
#define DBMS_C_RECOVERYMANAGER_H
#include "LogManager.h"
#include "BufferManager.h"
#include "recovery/LogRecord.h"
#include "LogIterator.h"
typedef struct Transaction Transaction;
typedef struct RecoveryManager{
    LogManager *logManager;
    BufferManager *bufferManager;
    Transaction *transaction;
    int txNum;
}RecoveryManager;
RecoveryManager *RecoveryManagerInit(Transaction*transaction,int txNum,LogManager *logManager,BufferManager *bufferManager);
#endif //DBMS_C_RECOVERYMANAGER_H
