//
// Created by yaohuayuan on 2024/11/10.
//

#include "RecoveryManager.h"
RecoveryManager *RecoveryManagerInit(Transaction*transaction,int txNum,LogManager *logManager,BufferManager *bufferManager){
    RecoveryManager *recoveryManager = malloc(sizeof(RecoveryManager));
    recoveryManager->transaction = transaction;
    recoveryManager->txNum = txNum;
    recoveryManager->logManager = logManager;
    recoveryManager->bufferManager = bufferManager;
    StartRecordWriteToLog(logManager,txNum);
    return recoveryManager;
}
void RecoveryCommit(RecoveryManager *recoveryManager){
    BufferManagerFlushAll(recoveryManager->bufferManager,recoveryManager->txNum);
    int lsn = CommitRecordWriteToLog(recoveryManager->logManager,recoveryManager->txNum);
    LogManagerFlushLSN(recoveryManager->logManager,lsn);
}
void RecoveryDoRollback(RecoveryManager*recoveryManager){
    LogIterator *logIterator = LogManager2LogManager(recoveryManager->logManager);
    while (LogIteratorNext(logIterator)){
       ByteBufferData*byteBufferData= LogIteratorNext(logIterator);
        ByteBuffer *buffer = bufferAllocate(byteBufferData->bytesData->length);
        buffer->data = byteBufferData->bytesData->data;
        buffer->type = byteBufferData->bytesData->type;
        LogRecord *logRecord = LogRecordInit(buffer);
        if(logRecord->LogRecordTxNum()==recoveryManager->txNum){
            if(logRecord->LogRecordOP() == LogRecordCode_START){
                return;
            }
            logRecord->LogRecordUnDo(recoveryManager->transaction);
        }
    }
}
void RecoveryRollback(RecoveryManager*recoveryManager){
    RecoveryDoRollback(recoveryManager);
    BufferManagerFlushAll(recoveryManager->bufferManager,recoveryManager->txNum);
    int lsn = RollbackRecordWriteToLog(recoveryManager->logManager,recoveryManager->txNum);
    LogManagerFlushLSN(recoveryManager->logManager,lsn);
}
int RecoverySetInt(RecoveryManager*recoveryManager,Buffer*buffer,int offset,int newVal){
    int oldVal;
    ByteBufferData *out = NULL;
    PageGetInt(buffer->page, offset, out);
    oldVal =*out->intData;
    BlockID blockId = buffer->blockId;
    return SetIntRecordWriteToLog(recoveryManager->logManager,recoveryManager->txNum,blockId,offset,oldVal);
}
int RecoverySetString(RecoveryManager*recoveryManager,Buffer*buffer,int offset,char* newVal){
    char*oldVal = PageGetString(buffer->page,offset);
    BlockID blockId = buffer->blockId;
    return SetStringRecordWriteToLog(recoveryManager->logManager,recoveryManager->txNum,blockId,offset,oldVal);
}
