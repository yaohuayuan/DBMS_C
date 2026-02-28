//
// Created by yaohuayuan on 2024/11/7.
//

#include "LogRecord.h"
#include "Transaction.h"
RollbackRecord* RollbackRecordInit(Page *page){
    int tpos = sizeof(int);
    RollbackRecord * rollbackRecord = malloc(sizeof(RollbackRecord));
    rollbackRecord->TxNum = PageGetInt(page,tpos);
    return rollbackRecord;
}
LogRecordCode RollbackRecordOP(Transaction*transaction){
    return LogRecordCode_ROLLBACK;
}
char* RollbackRecordToString(LogRecord *logRecord){
    RollbackRecord *rollbackRecord = logRecord->LogRecordData.rollbackRecord;
    char *str = NULL;
    sprintf(str,"<ROLLBACK %d >",rollbackRecord->TxNum);
    return str;
}
int RollbackRecordWriteToLog(LogManager*logManager,int TxNum){
    Page *page = PageInit(2*sizeof(int));
    PageSetInt(page,0,LogRecordCode_ROLLBACK);
    PageSetInt(page,sizeof (int),TxNum);
    return LogManagerAppend(logManager,page->buffer->data,page->buffer->size);
}
void RollbackRecordUnDo(Transaction*transaction,LogRecord*logRecord){}
int RollbackRecordTxNumber(LogRecord *logRecord){
    return logRecord->LogRecordData.rollbackRecord->TxNum;
}
StartRecord * StartRecordInit(Page*page){
    int tPos = sizeof (tPos);
    StartRecord *startRecord = malloc(sizeof (StartRecord));
    startRecord->TxNum = PageGetInt(page,tPos);
    return startRecord;
}
LogRecordCode StartRecordOP(){
    return LogRecordCode_START;
}
int StartRecordTxNumber(LogRecord *logRecord ){
    return logRecord->LogRecordData.startRecord->TxNum;
}
char * StartRecordTostring(LogRecord*logRecord){
    StartRecord *startRecord = logRecord->LogRecordData.startRecord;
    char *str = NULL;
    sprintf(str,"<START %d >",startRecord->TxNum);
    return str;
}
void StartRecordUnDo(Transaction*transaction,LogRecord*logRecord){}
int StartRecordWriteToLog(LogManager *logManager,int txNum){
    Page *page = PageInit(2*sizeof (int));
    PageSetInt(page,0,LogRecordCode_START);
    PageSetInt(page,sizeof (int),txNum);
    return LogManagerAppend(logManager,page->buffer->data,2*sizeof (int));
}
SetStringRecord* SetStringRecordInit(Page* page) {
    SetStringRecord* record = malloc(sizeof(SetStringRecord));
    int tpos = sizeof(int);
    record->TxNum = PageGetInt(page, tpos);
    int fpos = tpos + sizeof(int);
    CString* filename = PageGetString(page, fpos);
    int bpos = fpos + PageMaxLength(filename) ; // 假设 PageGetString 会返回 null 结尾的字符串
    int blknum ;

    blknum =  PageGetInt(page, bpos);
    record->BlockId = BlockIDInit(filename, blknum);
    int opos = bpos + sizeof(int);
    record->Offset = PageGetInt(page, opos);
    int vpos = opos + sizeof(int);
    CString *valCString = PageGetString(page, vpos);
    record->Val = CStringGetPtr(valCString);

    return record;
}
CommitRecord *CommitRecordInit(Page*page){
    CommitRecord *commitRecord = malloc(sizeof (CommitRecord));
    int tPos = sizeof (int);
    commitRecord->TxNum = PageGetInt(page,tPos);
    return commitRecord;
}
LogRecordCode CommitRecordOP(){
    return LogRecordCode_COMMIT;
}
void CommitUnDo(Transaction*transaction,LogRecord*logRecord){}
char * CommitRecordToString(LogRecord *logRecord) {
    CommitRecord* commitRecord = logRecord->LogRecordData.commitRecord;
    char* str = malloc(50 * sizeof(char));
    sprintf(str, "<COMMIT %d>", commitRecord->TxNum);
    return str;
}

// 写入 CommitRecord 到日志
int CommitRecordWriteToLog(LogManager* logManager, int txNum) {
    Page* page = PageInit(2 * sizeof(int));
    PageSetInt(page, 0, LogRecordCode_COMMIT);
    PageSetInt(page, sizeof(int), txNum);
    return LogManagerAppend(logManager, page->buffer->data,  2 * sizeof(int));
}
int CommitRecordTxNumber(LogRecord *logRecord){
    return logRecord->LogRecordData.commitRecord->TxNum;
}
// SetIntRecord 初始化
SetIntRecord* SetIntRecordInit(Page* page) {
    int tpos = sizeof(int);
    SetIntRecord* setIntRecord = malloc(sizeof(SetIntRecord));
    setIntRecord->TxNum =  PageGetInt(page, tpos);
    int fpos = tpos + sizeof(int);
    CString* filename = PageGetString(page, fpos);
    int bpos = fpos + PageMaxLength(filename);
    setIntRecord->BlockId = BlockIDInit(CStringCreateFromCString(filename),PageGetInt(page, bpos));
    int opos = bpos + sizeof(int);
    setIntRecord->Offset = PageGetInt(page, opos);
    int vpos = opos + sizeof(int);
    setIntRecord->Val = PageGetInt(page, vpos);
    return setIntRecord;
}

// SetIntRecord 的操作类型
LogRecordCode SetIntRecordOP() {
    return LogRecordCode_SETINT;
}

// SetIntRecord 转换为字符串
char* SetIntRecordToString(LogRecord *logRecord) {
    SetIntRecord *setIntRecord = logRecord->LogRecordData.setIntRecord;
    char* str = malloc(100 * sizeof(char));
    sprintf(str, "<SETINT %d %s %d %d %d>", setIntRecord->TxNum, CStringGetPtr(setIntRecord->BlockId->fileName),  setIntRecord->BlockId->BlockID, setIntRecord->Offset, setIntRecord->Val);
    return str;
}

// 写入 SetIntRecord 到日志
int SetIntRecordWriteToLog(LogManager* logManager, int txNum, BlockID *blockId, int offset, int val) {
    int tpos = sizeof(int);
    int fpos = tpos + sizeof(int);
    int bpos = fpos + PageMaxLength(blockId->fileName);
    int opos = bpos + sizeof(int);
    int vpos = opos + sizeof(int);
    int recLen = vpos + sizeof(int);
    Page* page = PageInit(recLen);
    PageSetInt(page, 0, LogRecordCode_SETINT);
    PageSetInt(page, tpos, txNum);
    PageSetString(page, fpos, blockId->fileName);
    PageSetInt(page, bpos, blockId->BlockID);
    PageSetInt(page, opos, offset);
    PageSetInt(page, vpos, val);
    return LogManagerAppend(logManager, page->buffer->data,  recLen);
}
void SetIntUndo(Transaction*transaction,LogRecord*logRecord){
    SetIntRecord *setIntRecord = logRecord->LogRecordData.setIntRecord;
    TransactionPin(transaction,setIntRecord->BlockId);
    TransactionSetInt(transaction,setIntRecord->BlockId,setIntRecord->Offset,setIntRecord->Val,false);
    TransactionUnPin(transaction,setIntRecord->BlockId);
}
int SetIntTxNumber(LogRecord*logRecord){
    return logRecord->LogRecordData.setIntRecord->TxNum;
}
CheckpointRecord* CheckpointRecordInit(Page* page) {
    CheckpointRecord* checkpointRecord = malloc(sizeof(CheckpointRecord));
    // CheckpointRecord 不包含任何数据，直接返回
    return checkpointRecord;
}

// CheckpointRecord 的操作类型
LogRecordCode CheckpointRecordOP() {
    return LogRecordCode_CHECKPOINT;
}
int CheckpointRecordTxNumber() {
    return -1;
}
void CheckpointUnDo(Transaction*transaction,LogRecord*logRecord){}
// CheckpointRecord 转换为字符串
char* CheckpointRecordToString(LogRecord *logRecord) {
    char* str = malloc(50 * sizeof(char));
    sprintf(str, "<CHECKPOINT>");
    return str;
}

// 写入 CheckpointRecord 到日志
int CheckpointRecordWriteToLog(LogManager* logManager) {
    // 创建一个页面用于存储日志记录
    Page* page = PageInit(sizeof(int)); // 只存储一个整数（操作码）
    PageSetInt(page, 0, LogRecordCode_CHECKPOINT); // 设置操作码为 CHECKPOINT
    // 将日志写入到日志管理器并返回最后的 LSN
    return LogManagerAppend(logManager, page->buffer->data, sizeof (int));
}
// 操作码
LogRecordCode SetStringRecordOP() {
    return LogRecordCode_SETSTRING;
}
void SetStringUndo(Transaction*transaction,LogRecord*logRecord){
    SetStringRecord *setStringRecord = logRecord->LogRecordData.setStringRecord;
    TransactionPin(transaction,setStringRecord->BlockId);
    CString *valCString = CStringCreateFromCStr(setStringRecord->Val);
    TransactionSetString(transaction,setStringRecord->BlockId,setStringRecord->Offset,valCString,false);
    CStringDestroy(valCString);
    TransactionUnPin(transaction,setStringRecord->BlockId);
}
int SetStringTxNumber(LogRecord *logRecord){
    return logRecord->LogRecordData.setStringRecord->TxNum;
}
int SetStringRecordWriteToLog(LogManager* logManager, int txNum, BlockID *blockId, int offset, CString * val) {
    int tpos = sizeof(int);
    int fpos = tpos + sizeof(int);
    int bpos = fpos + PageMaxLength(blockId->fileName);
    int opos = bpos + sizeof(int);
    int vpos = opos + sizeof(int);
    int recLen = vpos + PageMaxLength(blockId->fileName);
    Page* page = PageInit(recLen+5);
    PageSetInt(page, 0, LogRecordCode_SETSTRING);
    PageSetInt(page, tpos, txNum);
    PageSetString(page, fpos, blockId->fileName);
    PageSetInt(page, bpos, blockId->BlockID);
    PageSetInt(page, opos, offset);
    PageSetString(page, vpos, val);
    return LogManagerAppend(logManager, page->buffer->data, recLen);
}
// 生成字符串表示
char* SetStringRecordToString(LogRecord *logRecord) {
    SetStringRecord *record = logRecord->LogRecordData.setStringRecord;
    char* str = malloc(1024 * sizeof(char));  // 分配足够空间
    sprintf(str, "<SETSTRING %d %s %d %s>", record->TxNum, CStringGetPtr(BlockID2CString(record->BlockId)) , record->Offset, record->Val);
    return str;
}
LogRecord * LogRecordInit(ByteBuffer* buffer){
    Page *page = PageInitBytes(buffer);
    LogRecordCode code = PageGetInt(page, 0);
    LogRecord * logRecord = malloc(sizeof(LogRecord));

    switch (code) {
        case LogRecordCode_COMMIT:
            logRecord->LogRecordData.commitRecord = CommitRecordInit(page);
            logRecord->logRecordCode = LogRecordCode_COMMIT;
            logRecord->LogRecordOP = CommitRecordOP;
            logRecord->LogRecordUnDo=CommitUnDo;
            logRecord->LogRecordTxNum = CommitRecordTxNumber;
            logRecord->LogRecordToString = CommitRecordToString;
            return logRecord;
        case LogRecordCode_CHECKPOINT:
            logRecord->LogRecordData.checkpointRecord = CheckpointRecordInit(page);
            logRecord->logRecordCode = LogRecordCode_CHECKPOINT;
            logRecord->LogRecordOP = CheckpointRecordOP;
            logRecord->LogRecordTxNum = CheckpointRecordTxNumber;
            logRecord->LogRecordUnDo = CheckpointUnDo;
            logRecord->LogRecordToString = CheckpointRecordToString;
            return logRecord;
        case LogRecordCode_START:
            logRecord->LogRecordUnDo = StartRecordUnDo;
            logRecord->LogRecordTxNum = StartRecordTxNumber;
            logRecord->LogRecordOP = StartRecordOP;
            logRecord->logRecordCode = LogRecordCode_START;
            logRecord->LogRecordData.startRecord= StartRecordInit(page);
            logRecord->LogRecordToString = StartRecordTostring;
            return logRecord;
        case LogRecordCode_ROLLBACK:
            logRecord->LogRecordData.rollbackRecord = RollbackRecordInit(page);
            logRecord->logRecordCode=LogRecordCode_ROLLBACK;
            logRecord->LogRecordOP = RollbackRecordOP;
            logRecord->LogRecordTxNum = RollbackRecordTxNumber;
            logRecord->LogRecordUnDo = RollbackRecordUnDo;
            logRecord->LogRecordToString = RollbackRecordToString;
            return logRecord;
        case LogRecordCode_SETINT:
            logRecord->LogRecordData.setIntRecord = SetIntRecordInit(page);
            logRecord->LogRecordUnDo = SetIntUndo;
            logRecord->logRecordCode = LogRecordCode_SETINT;
            logRecord->LogRecordOP = SetIntRecordOP;
            logRecord->LogRecordTxNum = SetIntTxNumber;
            logRecord->LogRecordToString = SetIntRecordToString;
            return logRecord;
        case LogRecordCode_SETSTRING:
            logRecord->LogRecordData.setStringRecord = SetStringRecordInit(page);
            logRecord->LogRecordUnDo = SetStringUndo;
            logRecord->logRecordCode = LogRecordCode_SETSTRING;
            logRecord->LogRecordOP = SetStringRecordOP;
            logRecord->LogRecordTxNum = SetStringTxNumber;
            logRecord->LogRecordToString = SetStringRecordToString;
            return logRecord;
    }
}