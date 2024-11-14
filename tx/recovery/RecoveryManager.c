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
//            BlockID blockId;
//        BlockID_Init(&blockId,recoveryManager->logManager->currentBlockId.fileName,0);
//        Page *page = PageInit(400);
//        FileManagerRead(recoveryManager->logManager->fileManager,blockId,page);
//        ByteBufferData *byteBufferData = ByteBufferDataInit();
//        PageGetInt(page,0,byteBufferData);
//        int boundary=*byteBufferData->intData;
    LogIterator *logIterator = LogManager2LogManager(recoveryManager->logManager);
    while (LogIteratorHasNext(logIterator)){
       ByteBufferData*byteBufferData= LogIteratorNext(logIterator);
        ByteBuffer *buffer = bufferAllocate(byteBufferData->bytesData->length);
        buffer->data = byteBufferData->bytesData->data;
        buffer->type = byteBufferData->bytesData->type;
        LogRecord *logRecord = LogRecordInit(buffer);
        if(logRecord->LogRecordTxNum(logRecord)==recoveryManager->txNum){
            if(logRecord->LogRecordOP() == LogRecordCode_START){
                return;
            }
            logRecord->LogRecordUnDo(recoveryManager->transaction,logRecord);
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
    ByteBufferData *out = ByteBufferDataInit();
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
IntNode *IntNodeInit(){
    IntNode *head = malloc(sizeof(IntNode));
    head->next = NULL;
    return head;
}
void IntNodeAdd(IntNode*head,int data){
    IntNode * p =head;
    while (p->next){
        p = p->next;
    }
    IntNode *newNode = malloc(sizeof (IntNode));
    newNode->next = NULL;
    newNode->data = data;
    p->next = newNode;
}
bool IntNodeContains(IntNode*head,int data){
    IntNode *p = head->next;
    while (p){
        if(p->data==data)
            return true;
        p = p->next;
    }
    return false;

}
void RecoveryDoRecover(RecoveryManager*recoveryManager){
    IntNode *head = IntNodeInit();
    LogIterator *logIterator = LogManager2LogManager(recoveryManager->logManager);
    while (LogIteratorHasNext(logIterator)){
        ByteBufferData*byteBufferData= LogIteratorNext(logIterator);
        ByteBuffer *buffer = bufferAllocate(byteBufferData->bytesData->length);
        buffer->data = byteBufferData->bytesData->data;
        buffer->type = byteBufferData->bytesData->type;
        LogRecord *logRecord = LogRecordInit(buffer);
        if(logRecord->LogRecordOP() == LogRecordCode_CHECKPOINT)
            return;
        if(logRecord->LogRecordOP()==LogRecordCode_COMMIT||logRecord->LogRecordOP()==LogRecordCode_ROLLBACK){
            IntNodeAdd(head,logRecord->LogRecordTxNum(logRecord));
        }else if(!IntNodeContains(head,logRecord->LogRecordTxNum(logRecord))){
            logRecord->LogRecordUnDo(recoveryManager->transaction,logRecord);
        }
    }
}
void RecoveryRecovery(RecoveryManager*recoveryManager){
    RecoveryDoRecover(recoveryManager);
    BufferManagerFlushAll(recoveryManager->bufferManager,recoveryManager->txNum);
    int lsn = CheckpointRecordWriteToLog(recoveryManager->logManager);
    LogManagerFlushLSN(recoveryManager->logManager,lsn);
}