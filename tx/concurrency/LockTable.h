//
// Created by yaohuayuan on 2024/11/4.
//

#ifndef DBMS_C_LOCKTABLE_H
#define DBMS_C_LOCKTABLE_H
#include "map.h"
#include "time.h"
#include "BlockId.h"
#include "Error.h"
#define LockTableTime 1
/**
 * @brief LockTable 结构体，用于管理锁表，存储块的锁状态。
 */
typedef struct LockTable{
    map_int_t *Locks;  ///< 用于存储块锁信息的映射
}LockTable;

/**
 * @brief 初始化一个新的 LockTable 实例。
 *
 * @return 返回初始化后的 LockTable 指针。
 */
LockTable * LockTableInit();

/**
 * @brief 获取锁表中指定块的锁值。
 *
 * @param lockTable 指向 LockTable 实例的指针。
 * @param blockId 指向 BlockID 实例的指针。
 * @return 返回锁值。
 */
int LockTableGetLocalVal(LockTable * lockTable,BlockID  *blockId);

/**
 * @brief 检查等待时间是否过长。
 *
 * @param startTime 开始等待的时间戳。
 * @return 如果等待时间过长，返回 true；否则返回 false。
 */
bool LockTableWaitToLong(long long startTime);

/**
 * @brief 检查指定块是否持有排他锁（X锁）。
 *
 * @param lockTable 指向 LockTable 实例的指针。
 * @param blockId 指向 BlockID 实例的指针。
 * @return 如果持有排他锁，返回 true；否则返回 false。
 */
bool LockTableHasXLock(LockTable *lockTable,BlockID *blockId);

/**
 * @brief 检查指定块是否持有其他共享锁（S锁）。
 *
 * @param lockTable 指向 LockTable 实例的指针。
 * @param blockId 指向 BlockID 实例的指针。
 * @return 如果持有其他共享锁，返回 true；否则返回 false。
 */
bool LockTableHasSOtherLock(LockTable *lockTable,BlockID *blockId);

/**
 * @brief 为指定块获取排他锁（X锁）。
 *
 * @param lockTable 指向 LockTable 实例的指针。
 * @param blockId 指向 BlockID 实例的指针。
 * @param error 指向 Error 实例的指针，用于存储错误信息。
 */
void LockTableXLock(LockTable*lockTable,BlockID *blockId,Error *error);

/**
 * @brief 释放指定块的锁。
 *
 * @param lockTable 指向 LockTable 实例的指针。
 * @param blockId 指向 BlockID 实例的指针。
 */
void LockTableUnLock(LockTable*lockTable,BlockID *blockId);

/**
 * @brief 为指定块获取共享锁（S锁）。
 *
 * @param lockTable 指向 LockTable 实例的指针。
 * @param blockId 指向 BlockID 实例的指针。
 * @param error 指向 Error 实例的指针，用于存储错误信息。
 */
void LockTableSLock(LockTable*lockTable,BlockID *blockId,Error *error);

/**
 * @brief 将指定块的共享锁（S锁）升级为排他锁（X锁）。
 *
 * @param lockTable 指向 LockTable 实例的指针。
 * @param blockId 指向 BlockID 实例的指针。
 * @param error 指向 Error 实例的指针，用于存储错误信息。
 */
void LockTableUpgrade(LockTable *lockTable, BlockID *blockId, Error *error);
#endif //DBMS_C_LOCKTABLE_H
