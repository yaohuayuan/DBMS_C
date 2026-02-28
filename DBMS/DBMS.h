//
// Created by Lenovo on 2025/7/31.
//

#ifndef NEWDBMS_DBMS_H
#define NEWDBMS_DBMS_H
#include "FileManager.h"
#include "BufferManager.h"
#include "LogManager.h"
#include "Transaction.h"
#include "MetadataManager.h"
#include "Planner.h"
#include "windows.h"
#define SIMPLE_DB_INIT_VAL (-1)
#define BLOCK_SIZE 4096
#define BUFFER_SIZE 8
#define LOG_FILE "SimpleDB.log"
typedef struct SimpleDB{
    FileManager *fileManager;
    BufferManager *bufferManager;
    LogManager *logManager;
    MetadataMgr *metadataMgr;
    Planner *planer;
//    HANDLE bgThread;   // 后台刷盘线程
//    HANDLE bgEvent;    // 事件对象，用来唤醒
}SimpleDB;
/**
 * 初始化 SimpleDB 实例。
 *
 * @param dirname 数据库目录名。
 * @param blocksize 数据块大小。
 * @param buffersize 缓冲区大小。
 * @return 返回初始化后的 SimpleDB 指针。
 */
SimpleDB *SimpleDBInit(char *dirname,int blocksize,int buffersize);

/**
 * 创建一个新的事务。
 *
 * @param simpleDb SimpleDB 实例指针。
 * @return 返回新创建的事务指针。
 */
Transaction *SimpleDataNewTX(SimpleDB*simpleDb);

/**
 * 使用指定名称创建一个新的事务。
 *
 * @param simpleDb SimpleDB 实例指针。
 * @param name 事务名称。
 * @return 返回新创建的事务指针。
 */
Transaction *SimpleDataNewTXName(SimpleDB*simpleDb,char* *name);
#endif //NEWDBMS_DBMS_H
