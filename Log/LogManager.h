//
// Created by Lenovo on 2025/6/27.
//

#ifndef NEWDBMS_LOGMANAGER_H
#define NEWDBMS_LOGMANAGER_H
#include "FileManager.h"
typedef struct LogIterator LogIterator;
/**
 * @brief LogManager 结构体定义，用于管理日志文件。
 */
typedef struct {
    CString *logFile;            ///< 日志文件的路径
    FileManager *fileManager; ///< 指向文件管理器的指针，用于管理文件操作
    Page *logPage;            ///< 当前页的数据缓冲区
    BlockID *currentBlockId;   ///< 当前处理的日志块ID
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
LogManager* LogManagerInit(FileManager *fileManager, CString *logfile);

/**
 * @brief 在日志文件中追加一个新块。
 *
 * @param logManager 指向 LogManager 的指针。
 * @return 返回新创建的块ID。
 */
BlockID* LogManagerAppendNewBlock(LogManager* logManager);

/**
 * @brief 将当前日志内容刷新到磁盘。
 *
 * @param logManager 指向 LogManager 的指针。
 */
void LogManagerFlush(LogManager *logManager);

/**
 * @brief 创建一个新的 LogIterator 实例，用于遍历日志条目。
 *
 * @param logManager 指向 LogManager 的指针。
 * @return 返回初始化后的 LogIterator 指针。
 */
struct LogIterator* LogManager2LogManager(LogManager *logManager);

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
int LogManagerAppend(LogManager *logManager,const uint8_t *data,  uint32_t size);

typedef struct LogIterator {
    FileManager *fm;         ///< 指向文件管理器的指针，用于管理文件操作
    BlockID *blockId;         ///< 当前处理的日志块ID
    Page *page;              ///< 当前页的数据缓冲区
    int currentPos;          ///< 当前读取位置
    int boundary;            ///< 当前页的有效数据边界
} LogIterator;

/**
 * @brief 初始化一个新的 LogIterator 实例。
 *
 * @param fm 指向文件管理器的指针。
 * @param blockId 日志开始的块ID。
 * @return 返回初始化后的 LogIterator 指针。
 */
LogIterator* LogIteratorInit(FileManager *fm, BlockID* blockId);

/**
 * @brief 移动迭代器到指定的日志块。
 *
 * @param logIterator 指向 LogIterator 的指针。
 */
void LogIteratorMoveToBlock(LogIterator* logIterator);

/**
 * @brief 检查是否还有下一个日志条目可以读取。
 *
 * @param logIterator 指向 LogIterator 的指针。
 * @return 如果有下一个日志条目，则返回 true；否则返回 false。
 */
bool LogIteratorHasNext(LogIterator *logIterator);

/**
 * @brief 获取下一个日志条目。
 *
 * @param logIterator 指向 LogIterator 的指针。
 * @return 返回下一个日志条目的 ByteBufferData 指针；如果没有更多条目，则返回 NULL。
 */
ByteBuffer * LogIteratorNext(LogIterator *logIterator);



#endif //NEWDBMS_LOGMANAGER_H
