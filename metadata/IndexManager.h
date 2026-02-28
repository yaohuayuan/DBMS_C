//
// Created by yaohuayuan on 2024/11/21.
//

#ifndef DBMS_C_INDEXMANAGER_H
#define DBMS_C_INDEXMANAGER_H

#include "Transaction.h"
#include "TableManager.h"
#include "StatManager.h"
#include "Schema.h"
#include "Layout.h"
#include "TableScan.h"
#include "map.h"
#include "IndexInfo.h"
#include "CString.h"

/**
 * @brief 定义存储索引信息的哈希表类型。
 */
typedef map_t(IndexInfo) map_IndexInfo_t;

/**
 * @brief IndexManager 结构体定义，用于管理索引。
 */
typedef struct IndexMgr {
    Layout *layout;           ///< 表布局信息
    TableManager *tableManager; ///< 指向表管理器的指针
    StatManager *statManager; ///< 指向统计管理器的指针
} IndexManager;

/**
 * @brief 初始化一个新的 IndexManager 实例。
 *
 * @param isNew 如果是新创建，则为 true；否则为 false。
 * @param tableManager 指向表管理器的指针。
 * @param statManager 指向统计管理器的指针。
 * @param tx 指向当前事务的指针。
 * @return 返回初始化后的 IndexManager 指针。
 */
IndexManager* IndexMgrInit(bool isNew, TableManager *tableManager, StatManager *statManager, Transaction *tx);

/**
 * @brief 创建一个新的索引。
 *
 * @param indexMgr 指向 IndexManager 的指针。
 * @param idxname 索引名称。
 * @param tblname 表名称。
 * @param fldname 字段名称，该字段上创建了索引。
 * @param tx 指向当前事务的指针。
 */
void IndexMgrCreateIndex(IndexManager *indexMgr, CString *idxname, CString *tblname, CString *fldname, Transaction *tx);

/**
 * @brief 获取指定表的所有索引信息。
 *
 * @param indexMgr 指向 IndexManager 的指针。
 * @param tblname 表名称。
 * @param tx 指向当前事务的指针。
 * @return 返回包含索引信息的哈希表指针。
 */
map_IndexInfo_t* IndexMgrGetIndexInfo(IndexManager *indexMgr, CString *tblname, Transaction *tx);

/**
 * @brief 释放 IndexManager 实例及其资源。
 *
 * @param indexMgr 指向 IndexManager 的指针。
 */
void IndexMgrFree(IndexManager *indexMgr);

#endif //DBMS_C_INDEXMANAGER_H