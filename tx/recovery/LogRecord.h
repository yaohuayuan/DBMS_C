// Created by yaohuayuan on 2024/11/7.

#ifndef DBMS_C_LOGRECORD_H
#define DBMS_C_LOGRECORD_H

#include "Page.h"
#include "LogManager.h"
#include "Transaction.h"
typedef struct Transaction Transaction;
// 定义日志记录类型
typedef enum LogRecordCode {
    LogRecordCode_CHECKPOINT,
    LogRecordCode_START,
    LogRecordCode_COMMIT,
    LogRecordCode_ROLLBACK,
    LogRecordCode_SETINT,
    LogRecordCode_SETSTRING
} LogRecordCode;

// 定义日志记录结构体
typedef struct RollbackRecord {
    int TxNum;
} RollbackRecord;

typedef struct StartRecord {
    int TxNum;
} StartRecord;

typedef struct CommitRecord {
    int TxNum;
} CommitRecord;

typedef struct SetIntRecord {
    int TxNum, Offset, Val;
    BlockID BlockId;
} SetIntRecord;

typedef struct SetStringRecord {
    int TxNum, Offset;
    char *Val;
    BlockID BlockId;
} SetStringRecord;
typedef struct CheckpointRecord {
    // CheckpointRecord 不需要任何成员，因为它只包含操作类型
} CheckpointRecord;

typedef struct LogRecord{
    LogRecordCode logRecordCode;
    union LogRecordData{
        SetStringRecord *setStringRecord;
        CommitRecord *commitRecord;
        StartRecord *startRecord;
        SetIntRecord *setIntRecord;
        CheckpointRecord *checkpointRecord;
        RollbackRecord *rollbackRecord;
    }LogRecordData;
    LogRecordCode (*LogRecordOP)();
    int (*LogRecordTxNum)();
    void (*LogRecordUnDo)(Transaction *transaction,struct LogRecord*logRecord);
}LogRecord;
LogRecord * LogRecordInit(ByteBuffer* buffer);
// 函数声明
RollbackRecord* RollbackRecordInit(Page *page);
LogRecordCode RollbackRecordOP();
char* RollbackRecordToString(RollbackRecord *rollbackRecord);
int RollbackRecordWriteToLog(LogManager*logManager, int TxNum);

StartRecord* StartRecordInit(Page* page);
LogRecordCode StartRecordOP();
char* StartRecordToString(StartRecord* startRecord);
int StartRecordWriteToLog(LogManager* logManager, int txNum);

CommitRecord* CommitRecordInit(Page* page);
LogRecordCode CommitRecordOP();
char* CommitRecordToString(CommitRecord* commitRecord);
int CommitRecordWriteToLog(LogManager* logManager, int txNum);

SetIntRecord* SetIntRecordInit(Page* page);
LogRecordCode SetIntRecordOP();
char* SetIntRecordToString(SetIntRecord* setIntRecord);
int SetIntRecordWriteToLog(LogManager* logManager, int txNum, BlockID blockId, int offset, int val);

SetStringRecord* SetStringRecordInit(Page* page);
LogRecordCode SetStringRecordOP();
char* SetStringRecordToString(SetStringRecord* setStringRecord);
int SetStringRecordWriteToLog(LogManager* logManager, int txNum, BlockID blockId, int offset, char* val);
CheckpointRecord* CheckpointRecordInit(Page* page);
LogRecordCode CheckpointRecordOP();
int CheckpointRecordWriteToLog(LogManager* logManager);

#endif //DBMS_C_LOGRECORD_H
