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
typedef struct IntNode{
    int data;
    struct IntNode*next;
}IntNode;
RecoveryManager *RecoveryManagerInit(Transaction*transaction,int txNum,LogManager *logManager,BufferManager *bufferManager);
void RecoveryCommit(RecoveryManager *recoveryManager);
void RecoveryDoRollback(RecoveryManager*recoveryManager);
void RecoveryRollback(RecoveryManager*recoveryManager);
int RecoverySetInt(RecoveryManager*recoveryManager,Buffer*buffer,int offset,int newVal);
int RecoverySetString(RecoveryManager*recoveryManager,Buffer*buffer,int offset,char* newVal);
void RecoveryDoRecover(RecoveryManager*recoveryManager);
void RecoveryRecovery(RecoveryManager*recoveryManager);
#endif //DBMS_C_RECOVERYMANAGER_H
