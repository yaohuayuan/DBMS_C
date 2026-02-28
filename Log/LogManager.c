//
// Created by Lenovo on 2025/6/27.
//

#include "LogManager.h"
typedef struct {
    uint32_t length;
    uint32_t lsn;
} LogRecordHeader;

LogManager *LogManagerInit(FileManager*fileManager,CString *str){
    LogManager *logManager = malloc(sizeof (LogManager));
    logManager->fileManager = fileManager;
    logManager->logFile = CStringCreateFromCString(str);
    logManager->logPage = PageInit(logManager->fileManager->blockSize);
    logManager->LastSavedLSN = 0;
    logManager->latestLSN = 0;
    int logSize = FileManagerLength(fileManager,str);
    if(logSize==0){
        logManager->currentBlockId = LogManagerAppendNewBlock(logManager);
    }else{
        logManager->currentBlockId = BlockIDInit(logManager->logFile,logSize-1);
        FileManagerRead(logManager->fileManager,logManager->currentBlockId,logManager->logPage);
        // 读取最新的日志记录，获取最新的 LSN
        int boundary = PageGetInt(logManager->logPage, 0);
        if (boundary < logManager->fileManager->blockSize) {
            LogRecordHeader header;
            PageGetBytesRaw(logManager->logPage, boundary, (uint8_t*)&header, sizeof(header));
            logManager->latestLSN = header.lsn;
        }
    }
    return logManager;
}
BlockID *LogManagerAppendNewBlock(LogManager*logManager){
    BlockID*newBlockID = FileManagerAppend(logManager->fileManager,logManager->logFile);
    PageSetInt(logManager->logPage,0, logManager->fileManager->blockSize);
    FileManagerWrite(logManager->fileManager,newBlockID,logManager->logPage);
    return newBlockID;
}
void LogManagerFlush(LogManager*logManager){
    FileManagerWrite(logManager->fileManager,logManager->currentBlockId,logManager->logPage);
    logManager->LastSavedLSN = logManager->latestLSN;
}
int LogManagerAppend(LogManager* lm,const uint8_t* payload,uint32_t payloadSize) {

    uint32_t recordSize = sizeof(LogRecordHeader) + payloadSize;
    int boundary = PageGetInt(lm->logPage, 0);
    if (boundary < recordSize + sizeof(int)) {
        LogManagerFlush(lm);
        lm->currentBlockId = LogManagerAppendNewBlock(lm);
        boundary = PageGetInt(lm->logPage, 0);
    }
    int pos = boundary - recordSize;
    LogRecordHeader header = {
        .length = recordSize,
        .lsn = ++lm->latestLSN
    };
    PageSetBytesRaw(lm->logPage, pos,
                    (uint8_t*)&header, sizeof(header));

    PageSetBytesRaw(lm->logPage,
                    pos + sizeof(header),
                    payload, payloadSize);
    PageSetInt(lm->logPage, 0, pos);
    return header.lsn;
}

void LogManagerFlushLSN(LogManager * logManager,int lsn){
    if (lsn >= logManager->LastSavedLSN) {
        // 直接写入页面，而不是调用LogManagerFlush，因为LogManagerFlush会将LastSavedLSN设置为latestLSN
        FileManagerWrite(logManager->fileManager,logManager->currentBlockId,logManager->logPage);
        // LastSavedLSN应该等于请求的lsn，因为我们只应该flush到这个点
        logManager->LastSavedLSN = lsn;
    }
}
LogIterator *LogManager2LogManager(LogManager *logManager){
    // 在创建LogIterator之前，确保所有记录都被写入到磁盘
    // 这样，LogIterator就可以读取所有的记录，包括未刷新的记录
    FileManagerWrite(logManager->fileManager,logManager->currentBlockId,logManager->logPage);
    LogIterator *logIterator = LogIteratorInit(logManager->fileManager,logManager->currentBlockId);
    return logIterator;
}
LogIterator* LogIteratorInit(FileManager *fm, BlockID* blockId){
    LogIterator *logIterator = malloc(sizeof(LogIterator));
    logIterator->page = PageInit(fm->blockSize);
    logIterator->blockId = BlockIDInit(blockId->fileName,blockId->BlockID);
    logIterator->fm = fm;
    LogIteratorMoveToBlock(logIterator);
    return logIterator;
}
void LogIteratorMoveToBlock(LogIterator* logIterator){
    FileManagerRead(logIterator->fm,logIterator->blockId,logIterator->page);
    logIterator->boundary = PageGetInt(logIterator->page,0);
    // 当前块的记录是从边界位置开始存储的
    logIterator->currentPos = logIterator->boundary;
}
bool LogIteratorHasNext(LogIterator *logIterator){
    // 如果当前块还有记录，返回true
    if (logIterator->currentPos < logIterator->fm->blockSize) {
        // 检查是否有足够的空间来存储完整的记录
        // 至少需要一个 LogRecordHeader 的大小
        if (logIterator->currentPos + sizeof(LogRecordHeader) <= logIterator->fm->blockSize) {
            return true;
        }
    }
    // 如果当前块没有记录，检查前一个块
    if (logIterator->blockId->BlockID > 0) {
        // 移动到前一个块
        BlockID *prevBlockId = BlockIDInit(logIterator->blockId->fileName, logIterator->blockId->BlockID - 1);
        Page *prevPage = PageInit(logIterator->fm->blockSize);
        FileManagerRead(logIterator->fm, prevBlockId, prevPage);
        int prevBoundary = PageGetInt(prevPage, 0);
        free(prevPage);
        // 如果前一个块有记录，返回true
        return prevBoundary < logIterator->fm->blockSize;
    }
    // 没有更多记录了，返回false
    return false;
}
ByteBuffer* LogIteratorNext(LogIterator* it) {

    if (it->currentPos == it->fm->blockSize) {
        it->blockId->BlockID--;
        LogIteratorMoveToBlock(it);
    }

    LogRecordHeader header;
    PageGetBytesRaw(it->page,
                    it->currentPos,
                    (uint8_t*)&header,
                    sizeof(LogRecordHeader));

    uint32_t payloadSize =
        header.length - sizeof(LogRecordHeader);

    uint8_t* payload = malloc(payloadSize);

    PageGetBytesRaw(it->page,
        it->currentPos + sizeof(LogRecordHeader),
        payload,
        payloadSize);

    it->currentPos += header.length;

    return bufferInitByteSize(payloadSize, payload);
}
