//
// Created by yaohuayuan on 2024/11/10.
//

#include "Transaction.h"
#include "recovery/RecoveryManager.h"

int TransactionNextTxNumber(Transaction *transaction) {
    transaction->nextTxNum++;
    return transaction->nextTxNum;
}
Transaction* TransactionInit(FileManager*fileManager, LogManager*logManager, BufferManager*bufferManager){
    Transaction *transaction = malloc(sizeof (Transaction));
    transaction->END_OF_FILE=-1;
    transaction->nextTxNum= 0;
    transaction->txNum = TransactionNextTxNumber(transaction);
    transaction->fileManager = fileManager;
    transaction->bufferManager = bufferManager;
    transaction->recoveryManager = RecoveryManagerInit(transaction,transaction->txNum,logManager,bufferManager);
    transaction->conCurrencyManager = ConCurrencyManagerInit();
    transaction->bufferList = BufferListInit(bufferManager);
    return transaction;
}



