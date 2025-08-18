//
// Created by Lenovo on 2025/7/16.
//

#ifndef NEWDBMS_BUFFER_H
#define NEWDBMS_BUFFER_H

#include "FileManager.h"
#include "LogManager.h"
#include "Page.h"
#include "time.h"
typedef struct Buffer{
    FileManager *fileManager;
    LogManager *logManager;
    Page *page;
    BlockID *blockId;
    int pins;
    int txNum;
    int lsn;
    time_t lastUsed;   // 最近使用时间
}Buffer;
/**
 * 初始化一个新的 Buffer。
 *
 * @param fileManager 指向文件管理器的指针，用于文件操作。
 * @param logManager 指向日志管理器的指针，用于日志记录。
 * @return 返回一个初始化后的 Buffer 指针。
 */
Buffer *BufferInit(FileManager *fileManager, LogManager *logManager);

/**
 * 将 Buffer 标记为已修改，并记录事务号和日志序列号。
 *
 * @param buffer 指向需要设置为修改状态的 Buffer。
 * @param txNum 当前事务号，表示正在操作该缓冲区的事务。
 * @param lsn 日志序列号，标识该缓冲区对应的日志操作。
 * @return 无
 */
void BufferSetModified(Buffer *buffer, int txNum, int lsn);

/**
 * 将 Buffer 中的修改内容刷新到磁盘，确保数据持久化。
 *
 * @param buffer 指向需要刷新的 Buffer。
 * @return 无
 */
void BufferFlush(Buffer *buffer);

/**
 * 检查 Buffer 是否已经被 Pin（即被引用）。
 *
 * @param buffer 指向需要检查的 Buffer。
 * @return 如果 Buffer 已被 Pin（即被引用），返回 true；否则返回 false。
 */
bool BufferIsPinned(Buffer *buffer);

/**
 * 将 Buffer 引用计数增加，表示该缓冲区被引用。
 *
 * @param buffer 指向需要 Pin 的 Buffer。
 * @return 无
 */
void BufferPin(Buffer *buffer);

/**
 * 将 Buffer 引用计数减少，表示该缓冲区不再被引用。
 *
 * @param buffer 指向需要 UnPin 的 Buffer。
 * @return 无
 */
void BufferUnPin(Buffer *buffer);

/**
 * 将 Buffer 分配给指定的块 ID。
 *
 * @param buffer 指向需要分配块 ID 的 Buffer。
 * @param blockId 要分配给 Buffer 的块 ID。
 * @return 无
 */
void BufferAssignToBlock(Buffer *buffer, BlockID *blockId);

#endif //NEWDBMS_BUFFER_H
