//
// Created by yaohuayuan on 2024/11/10.
//

#ifndef DBMS_C_TRANSACTION_H
#define DBMS_C_TRANSACTION_H
#include "concurrency/ConcurrencyManager.h"
#include "BufferManager.h"
#include "BufferList.h"
# define Transaction_END_OF_FILE (-1)
typedef struct RecoveryManager RecoveryManager;
typedef struct Transaction{

    RecoveryManager *recoveryManager;
    ConCurrencyManager *conCurrencyManager;
    BufferManager *bufferManager;
    FileManager *fileManager;
    int txNum;
    BufferList *bufferList;
}Transaction;
Transaction* TransactionInit(FileManager*fileManager, LogManager*logManager, BufferManager*bufferManager);
void TransactionCommit(Transaction*transaction);
void TransactionRollback(Transaction*transaction);
void TransactionRecover(Transaction*transaction);
void TransactionPin(Transaction *transaction,BlockID blockId);
void TransactionUnPin(Transaction *transaction,BlockID blockId);
int TransactionGetInt(Transaction*transaction,BlockID blockId,int offset);
char *TransactionGetString(Transaction*transaction,BlockID blockId,int offset);
void TransactionSetInt(Transaction*transaction,BlockID blockId,int offset,int val,bool okToLog);
void TransactionSetString(Transaction*transaction,BlockID blockId,int offset,char* val,bool okToLog);
int TransactionSize(Transaction *transaction,char *fileName);
BlockID TransactionAppend(Transaction *transaction,char*fileName);
int TransactionBlockSize(Transaction*transaction);
int TransactionAvailableBuffs(Transaction*transaction);
#endif //DBMS_C_TRANSACTION_H
