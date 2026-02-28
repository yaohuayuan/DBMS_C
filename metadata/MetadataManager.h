//
// Created by yaohuayuan on 2024/11/21.
//

#ifndef DBMS_C_METADATAMANAGER_H
#define DBMS_C_METADATAMANAGER_H

#include "Schema.h"
#include "Layout.h"
#include "Transaction.h"
#include "TableManager.h"
#include "IndexManager.h"
#include "ViewManager.h"
#include "map.h"
#include "CString.h"

/**
 * @brief MetadataMgr 结构体定义，用于管理数据库元数据。
 */
typedef struct MetadataMgr {
    TableManager *tblMgr;     ///< 指向表管理器的指针
    ViewManager *viewMgr;     ///< 指向视图管理器的指针
    StatManager *statMgr;    ///< 指向统计管理器的指针
    IndexManager *idxMgr;     ///< 指向索引管理器的指针
} MetadataMgr;

/**
 * @brief 初始化一个新的 MetadataMgr 实例。
 *
 * @param isNew 如果是新创建，则为 true；否则为 false。
 * @param tx 指向当前事务的指针。
 * @return 返回初始化后的 MetadataMgr 指针。
 */
MetadataMgr* MetadataMgrInit(bool isNew, Transaction *tx);

/**
 * @brief 创建一个新的表。
 *
 * @param mdMgr 指向 MetadataMgr 的指针。
 * @param tblname 表名称。
 * @param schema 表模式信息。
 * @param tx 指向当前事务的指针。
 */
void MetadataMgrCreateTable(MetadataMgr *mdMgr, CString *tblname, Schema *schema, Transaction *tx);

/**
 * @brief 获取指定表的布局信息。
 *
 * @param mdMgr 指向 MetadataMgr 的指针。
 * @param tblname 表名称。
 * @param tx 指向当前事务的指针。
 * @return 返回表布局信息指针。
 */
Layout* MetadataMgrGetLayout(MetadataMgr *mdMgr, CString *tblname, Transaction *tx);

/**
 * @brief 创建一个新的视图。
 *
 * @param mdMgr 指向 MetadataMgr 的指针。
 * @param viewname 视图名称。
 * @param viewdef 视图定义（SQL语句）。
 * @param tx 指向当前事务的指针。
 */
void MetadataMgrCreateView(MetadataMgr *mdMgr, CString *viewname, CString *viewdef, Transaction *tx);

/**
 * @brief 获取指定视图的定义。
 *
 * @param mdMgr 指向 MetadataMgr 的指针。
 * @param viewname 视图名称。
 * @param tx 指向当前事务的指针。
 * @return 返回视图定义字符串。
 */
CString* MetadataMgrGetViewDef(MetadataMgr *mdMgr, CString *viewname, Transaction *tx);

/**
 * @brief 创建一个新的索引。
 *
 * @param mdMgr 指向 MetadataMgr 的指针。
 * @param idxname 索引名称。
 * @param tblname 表名称。
 * @param fldname 字段名称，该字段上创建了索引。
 * @param tx 指向当前事务的指针。
 */
void MetadataMgrCreateIndex(MetadataMgr *mdMgr, CString *idxname, CString *tblname, CString *fldname, Transaction *tx);

/**
 * @brief 获取指定表的所有索引信息。
 *
 * @param mdMgr 指向 MetadataMgr 的指针。
 * @param tblname 表名称。
 * @param tx 指向当前事务的指针。
 * @return 返回包含索引信息的哈希表指针。
 */
map_IndexInfo_t* MetadataManagerGetIndexInfo(MetadataMgr *mdMgr, CString *tblname, Transaction *tx);

/**
 * @brief 获取指定表的统计信息。
 *
 * @param mdMgr 指向 MetadataMgr 的指针。
 * @param tblname 表名称。
 * @param layout 表布局信息。
 * @param tx 指向当前事务的指针。
 * @return 返回统计信息指针。
 */
StatInfo* MetadataMgrGetStatInfo(MetadataMgr *mdMgr, CString *tblname, Layout *layout, Transaction *tx);

/**
 * @brief 释放 MetadataMgr 实例及其资源。
 *
 * @param mdMgr 指向 MetadataMgr 的指针。
 */
void MetadataMgrFree(MetadataMgr *mdMgr);

#endif //DBMS_C_METADATAMANAGER_H