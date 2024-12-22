//
// Created by yaohuayuan on 2024/10/8.
//

#ifndef DBMS_C_BUFFERMANAGER_H
#define DBMS_C_BUFFERMANAGER_H

#include "Buffer.h"
#include "time.h"

// 定义 BufferManager 结构体，管理所有缓存的缓冲区
typedef struct BufferManager {
    Buffer **bufferPool;  // 缓冲池，存储所有的 Buffer 对象
    int bufferSize;       // 缓冲池的大小，即 Buffer 数组的最大容量
    int numAvailable;     // 当前可用的 Buffer 数量
} BufferManager;

#define MAX_TIME  10  // 最大等待时间，用于超时机制（单位：秒）

/**
 * 初始化 BufferManager。
 *
 * @param fileManager 文件管理器，用于文件操作。
 * @param logManager 日志管理器，用于记录日志。
 * @param numBuffs 缓冲池的大小，即要初始化的 Buffer 数量。
 * @return 返回一个初始化后的 BufferManager 指针。
 */
BufferManager *BufferManagerInit(FileManager *fileManager, LogManager *logManager, int numBuffs);

/**
 * 刷新 BufferManager 中所有缓冲区的内容，确保数据持久化。
 *
 * @param bufferManager 指向要刷新的 BufferManager。
 * @param tx 当前事务号。
 * @return 无
 */
void BufferManagerFlushAll(BufferManager *bufferManager, int tx);

/**
 * 将指定的 Buffer 从 BufferManager 中取消引用（即 Unpin 操作）。
 *
 * @param bufferManager 指向 BufferManager 的指针。
 * @param buffer 要取消引用的 Buffer。
 * @return 无
 */
void BufferManagerUnpin(BufferManager *bufferManager, Buffer *buffer);

/**
 * 在 BufferManager 中查找已存在的缓存区，如果该缓存区已被加载则返回对应的 Buffer。
 *
 * @param bufferManager 指向 BufferManager 的指针。
 * @param blockId 要查找的块 ID。
 * @return 如果找到对应的 Buffer，返回 Buffer 指针；否则返回 NULL。
 */
Buffer* BufferManagerFindExistingBuffer(BufferManager *bufferManager, BlockID blockId);

/**
 * 选择一个未被 Pin 的 Buffer（即可以被分配给新请求的缓冲区）。
 *
 * @param bufferManager 指向 BufferManager 的指针。
 * @return 返回一个未被 Pin 的 Buffer 指针；如果没有可用的缓冲区，则返回 NULL。
 */
Buffer* BufferManagerChooseUnPinnedBuffer(BufferManager* bufferManager);

/**
 * 尝试将指定块 ID 的 Buffer 加入缓冲池，如果找到了则进行 Pin 操作。
 *
 * @param bufferManager 指向 BufferManager 的指针。
 * @param blockId 要 Pin 的块 ID。
 * @return 如果操作成功，返回对应的 Buffer 指针；否则返回 NULL。
 */
Buffer* BufferManagerTryToPin(BufferManager *bufferManager, BlockID blockId);

/**
 * 检查操作是否超时。
 *
 * @param startTime 操作开始的时间戳（以秒为单位）。
 * @return 如果操作已超时（超过最大等待时间），返回 true；否则返回 false。
 */
bool BufferManagerWaitTooLong(long startTime);

/**
 * 将指定块 ID 的 Buffer 加入缓冲池并进行 Pin 操作。
 *
 * @param bufferManager 指向 BufferManager 的指针。
 * @param blockId 要 Pin 的块 ID。
 * @return 返回 Pin 操作后的 Buffer 指针。
 */
Buffer *BufferManagerPin(BufferManager *bufferManager, BlockID blockId);

#endif //DBMS_C_BUFFERMANAGER_H
