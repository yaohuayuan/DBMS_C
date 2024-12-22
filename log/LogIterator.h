//
// Created by yaohuayuan on 2024/9/22.
//

#ifndef DBMS_C_LOGITERATOR_H
#define DBMS_C_LOGITERATOR_H

#include "FileManager.h"

/**
 * @brief LogIterator 结构体定义，用于遍历日志文件。
 */
typedef struct {
    FileManager *fm;         ///< 指向文件管理器的指针，用于管理文件操作
    BlockID blockId;         ///< 当前处理的日志块ID
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
LogIterator* LogIteratorInit(FileManager *fm, BlockID blockId);

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
ByteBufferData* LogIteratorNext(LogIterator *logIterator);

#endif //DBMS_C_LOGITERATOR_H