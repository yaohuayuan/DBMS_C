//
// Created by yaohuayuan on 2024/9/23.
//

#ifndef DBMS_C_LOGMANAGER_H
#define DBMS_C_LOGMANAGER_H

#include "FileManager.h"
#include "LogIterator.h"

/**
 * @brief LogManager 结构体定义，用于管理日志文件。
 */
typedef struct {
    char *logFile;            ///< 日志文件的路径
    FileManager *fileManager; ///< 指向文件管理器的指针，用于管理文件操作
    Page *logPage;            ///< 当前页的数据缓冲区
    BlockID currentBlockId;   ///< 当前处理的日志块ID
    int latestLSN;            ///< 最新的日志序列号 (Log Sequence Number)
    int LastSavedLSN;         ///< 上次保存的日志序列号
} LogManager;

/**
 * @brief 初始化一个新的 LogManager 实例。
 *
 * @param fileManager 指向文件管理器的指针。
 * @param logfile 日志文件的路径。
 * @return 返回初始化后的 LogManager 指针。
 */
LogManager* LogManagerInit(FileManager *fileManager, char *logfile);

/**
 * @brief 在日志文件中追加一个新块。
 *
 * @param logManager 指向 LogManager 的指针。
 * @return 返回新创建的块ID。
 */
BlockID LogManagerAppendNewBlock(LogManager* logManager);

/**
 * @brief 将当前日志内容刷新到磁盘。
 *
 * @param logManager 指向 LogManager 的指针。
 */
void LogManagerFlush(LogManager *logManager);

/**
 * @brief 将 LogManager 转换为 LogIterator，以便遍历日志条目。
 *
 * @param logManager 指向 LogManager 的指针。
 * @return 返回初始化后的 LogIterator 指针。
 */
LogIterator* LogManager2LogManager(LogManager *logManager);

/**
 * @brief 刷新指定 LSN 的日志条目到磁盘。
 *
 * @param logManager 指向 LogManager 的指针。
 * @param lsn 要刷新的日志序列号。
 */
void LogManagerFlushLSN(LogManager *logManager, int lsn);

/**
 * @brief 向日志文件中追加数据。
 *
 * @param logManager 指向 LogManager 的指针。
 * @param data 要写入的数据。
 * @param type 数据类型。
 * @param size 数据大小（字节数）。
 * @return 返回追加后的最新 LSN。
 */
int LogManagerAppend(LogManager *logManager, uint8_t *data, uint8_t *type, int size);

#endif //DBMS_C_LOGMANAGER_H