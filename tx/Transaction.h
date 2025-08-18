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
typedef enum {
    TX_TRANSACTION_COMMIT,  // 已提交
    TX_TRANSACTION_ROLLBACK,  // 已回滚
    TX_TRANSACTION_RUN,  // 正在运行
    TX_TRANSACTION_RECOVERY,
} TransactionStatus;
typedef struct Transaction{
    TransactionStatus code;
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
void TransactionPin(Transaction *transaction,BlockID *blockId);
void TransactionUnPin(Transaction *transaction,BlockID *blockId);
int TransactionGetInt(Transaction*transaction,BlockID *blockId,int offset);
CString *TransactionGetString(Transaction*transaction,BlockID *blockId,int offset);
void TransactionSetInt(Transaction*transaction,BlockID *blockId,int offset,int val,bool okToLog);
void TransactionSetString(Transaction*transaction,BlockID *blockId,int offset,CString * val,bool okToLog);
int TransactionSize(Transaction *transaction, CString *fileName);
BlockID* TransactionAppend(Transaction *transaction,CString*fileName);
int TransactionBlockSize(Transaction*transaction);
int TransactionAvailableBuffs(Transaction*transaction);
CString * TransactionToSting(Transaction*transaction);
#endif //DBMS_C_TRANSACTION_H
