//
// Created by yaohuayuan on 2024/11/4.
//

#ifndef DBMS_C_CONCURRENCYMANAGER_H
#define DBMS_C_CONCURRENCYMANAGER_H
#include "map.h"
#include "LockTable.h"
/**
 * @brief ConCurrencyManager 结构体，用于管理事务的并发控制。
 */
typedef struct ConCurrencyManager{
    map_str_t * mapStr;  ///< 用于存储锁信息的映射
}ConCurrencyManager;

/**
 * @brief 初始化一个新的 ConCurrencyManager 实例。
 *
 * @return 返回初始化后的 ConCurrencyManager 指针。
 */
ConCurrencyManager * ConCurrencyManagerInit();

/**
 * @brief 获取指定块的共享锁（S锁）。
 *
 * @param conCurrencyManager 指向 ConCurrencyManager 实例的指针。
 * @param blockId 指向 BlockID 实例的指针，表示要获取锁的块。
 */
void ConCurrencyManagerSLock(ConCurrencyManager *conCurrencyManager,BlockID *blockId);

/**
 * @brief 检查当前事务是否持有指定块的排他锁（X锁）。
 *
 * @param conCurrencyManager 指向 ConCurrencyManager 实例的指针。
 * @param blockId 指向 BlockID 实例的指针，表示要检查的块。
 * @return 如果持有排他锁，返回 true；否则返回 false。
 */
bool ConCurrencyManagerHasXLock(ConCurrencyManager *conCurrencyManager,BlockID *blockId);

/**
 * @brief 获取指定块的排他锁（X锁）。
 *
 * @param conCurrencyManager 指向 ConCurrencyManager 实例的指针。
 * @param blockId 指向 BlockID 实例的指针，表示要获取锁的块。
 */
void ConCurrencyManagerXLock(ConCurrencyManager *conCurrencyManager,BlockID* blockId);

/**
 * @brief 释放当前事务持有的所有锁。
 *
 * @param conCurrencyManager 指向 ConCurrencyManager 实例的指针。
 */
void ConCurrencyManagerRelease(ConCurrencyManager *conCurrencyManager);
#endif //DBMS_C_CONCURRENCYMANAGER_H
