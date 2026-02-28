//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_STATMANAGER_H
#define DBMS_C_STATMANAGER_H

#include "TableManager.h"
#include "Transaction.h"
#include "StatInfo.h"
#include <stdbool.h>
#include "map.h"

/**
 * @brief 定义存储统计信息的哈希表类型。
 */
typedef map_t(StatInfo) map_StartInfo_t;

/**
 * @brief StatManager 结构体定义，用于管理统计数据。
 */
typedef struct {
    TableManager *tblMgr;         ///< 指向表管理器的指针
    map_StartInfo_t *tablestats;  ///< 存储表统计信息的哈希表
    int numcalls;                 ///< 统计信息被访问的次数
} StatManager;

/**
 * @brief 初始化一个新的 StatManager 实例。
 *
 * @param tblMgr 指向表管理器的指针。
 * @param tx 指向当前事务的指针。
 * @return 返回初始化后的 StatManager 指针。
 */
StatManager* StartManagerInit(TableManager *tblMgr, Transaction *tx);

/**
 * @brief 获取指定表的统计信息。
 *
 * @param sm 指向 StatManager 的指针。
 * @param tblname 表名称。
 * @param layout 表布局信息。
 * @param tx 指向当前事务的指针。
 * @return 返回统计信息指针；如果未找到，则返回 NULL。
 */
StatInfo* StartManagerGetStatInfo(StatManager *sm, char *tblname, Layout *layout, Transaction *tx);

/**
 * @brief 刷新所有表的统计信息。
 *
 * @param sm 指向 StatManager 的指针。
 * @param tx 指向当前事务的指针。
 */
void StartManagerRefreshStatistics(StatManager *sm, Transaction *tx);

/**
 * @brief 计算并更新指定表的统计信息。
 *
 * @param sm 指向 StatManager 的指针。
 * @param tblname 表名称。
 * @param layout 表布局信息。
 * @param tx 指向当前事务的指针。
 * @return 返回计算后的统计信息指针。
 */
StatInfo* StartManagerCalcTableStats(StatManager *sm, char *tblname, Layout *layout, Transaction *tx);

/**
 * @brief 释放 StatManager 实例及其资源。
 *
 * @param sm 指向 StatManager 的指针。
 */
void StartManagerFree(StatManager *sm);

#endif //DBMS_C_STATMANAGER_H