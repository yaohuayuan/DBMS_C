//
// Created by yaohuayuan on 2024/11/10.
//

#include "Transaction.h"
#include "recovery/RecoveryManager.h"
int static nextTxNum=0;
int TransactionNextTxNumber(Transaction *transaction) {
    nextTxNum++;
    return nextTxNum;
}
Transaction* TransactionInit(FileManager*fileManager, LogManager*logManager, BufferManager*bufferManager){
    Transaction *transaction = malloc(sizeof (Transaction));
    transaction->txNum = TransactionNextTxNumber(transaction);
    transaction->fileManager = fileManager;
    transaction->bufferManager = bufferManager;
    transaction->recoveryManager = RecoveryManagerInit(transaction,transaction->txNum,logManager,bufferManager);
    transaction->conCurrencyManager = ConCurrencyManagerInit();
    transaction->bufferList = BufferListInit(bufferManager);
    return transaction;
}
void TransactionCommit(Transaction*transaction){
    RecoveryCommit(transaction->recoveryManager);
    printf("transaction %d commit\n",transaction->txNum);
    ConCurrencyManagerRelease(transaction->conCurrencyManager);
    BufferListUnpinAll(transaction->bufferList);
}
void TransactionRollback(Transaction*transaction){
    RecoveryRollback(transaction->recoveryManager);
    printf("transaction %d rollback\n",transaction->txNum);
    ConCurrencyManagerRelease(transaction->conCurrencyManager);
    BufferListUnpinAll(transaction->bufferList);
}
void TransactionRecover(Transaction*transaction){
    BufferManagerFlushAll(transaction->bufferManager,transaction->txNum);
    RecoveryRecovery(transaction->recoveryManager);
}
void TransactionPin(Transaction *transaction,BlockID blockId){
    BufferListPin(transaction->bufferList,blockId);
}
void TransactionUnPin(Transaction *transaction,BlockID blockId){
    BufferListUnPin(transaction->bufferList,blockId);
}
int TransactionGetInt(Transaction*transaction,BlockID blockId,int offset){
    ConCurrencyManagerSLock(transaction->conCurrencyManager,blockId);
    Buffer *buffer = BufferListGetBuffer(transaction->bufferList,blockId);
    ByteBufferData *out = ByteBufferDataInit();
    PageGetInt(buffer->page, offset, out);
    return  *out->intData;
}
char *TransactionGetString(Transaction*transaction,BlockID blockId,int offset){
    ConCurrencyManagerSLock(transaction->conCurrencyManager,blockId);
    Buffer *buffer = BufferListGetBuffer(transaction->bufferList,blockId);
    return PageGetString(buffer->page,offset);
}

void TransactionSetInt(Transaction*transaction,BlockID blockId,int offset,int val,bool okToLog){
    ConCurrencyManagerXLock(transaction->conCurrencyManager,blockId);
    Buffer *buffer = BufferListGetBuffer(transaction->bufferList,blockId);
    int lsn = -1;
    if(okToLog){
        lsn = RecoverySetInt(transaction->recoveryManager,buffer,offset,val);
    }
    Page *page = buffer->page;
    PageSetInt(page,offset,val);
    BufferSetModified(buffer,transaction->txNum,lsn);
}
void TransactionSetString(Transaction*transaction,BlockID blockId,int offset,char* val,bool okToLog){
    ConCurrencyManagerXLock(transaction->conCurrencyManager,blockId);
    Buffer *buffer = BufferListGetBuffer(transaction->bufferList,blockId);
    int lsn = -1;
    if(okToLog){
        lsn = RecoverySetString(transaction->recoveryManager,buffer,offset,val);
    }
    Page *page = buffer->page;
    PageSetString(page,offset,val);
    BufferSetModified(buffer,transaction->txNum,lsn);
}
int TransactionSize(Transaction *transaction,const char *fileName){
    BlockID blockId;
    BlockID_Init(&blockId,fileName,Transaction_END_OF_FILE);
    ConCurrencyManagerSLock(transaction->conCurrencyManager,blockId);
    return FileManagerLength(transaction->fileManager,fileName);
}
BlockID TransactionAppend(Transaction *transaction,char*fileName){
    BlockID blockId;
    BlockID_Init(&blockId,fileName,Transaction_END_OF_FILE);
    ConCurrencyManagerXLock(transaction->conCurrencyManager,blockId);
    return FileManagerAppend(transaction->fileManager,fileName);
}
int TransactionBlockSize(Transaction*transaction){
    return transaction->fileManager->blockSize;
}
int TransactionAvailableBuffs(Transaction*transaction){
    return transaction->bufferManager->numAvailable;
}