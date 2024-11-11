//
// Created by yaohuayuan on 2024/11/10.
//

#ifndef DBMS_C_TRANSACTION_H
#define DBMS_C_TRANSACTION_H
#include "concurrency/ConcurrencyManager.h"
#include "BufferManager.h"
#include "BufferList.h"

typedef struct RecoveryManager RecoveryManager;
typedef struct Transaction{
    int nextTxNum;
    int END_OF_FILE;
    RecoveryManager *recoveryManager;
    ConCurrencyManager *conCurrencyManager;
    BufferManager *bufferManager;
    FileManager *fileManager;
    int txNum;
    BufferList *bufferList;
}Transaction;
Transaction* TransactionInit(FileManager*fileManager, LogManager*logManager, BufferManager*bufferManager);
#endif //DBMS_C_TRANSACTION_H
