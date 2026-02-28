// Created by yaohuayuan on 2024/11/19.

#ifndef DBMS_C_TABLEMANAGER_H
#define DBMS_C_TABLEMANAGER_H

#include "Layout.h"
#include "Transaction.h"
#include "TableScan.h"
#include "CString.h"

#define TABLE_MANAGER_MAX_NAME 16 ///< 定义表名最大长度

/**
 * @brief TableManager 结构体，用于管理数据库中的表。
 */
typedef struct TableManager {
    Layout *tableCatalogLayout; ///< 表目录的布局
    Layout *fieldCatalogLayout; ///< 字段目录的布局
} TableManager;

/**
 * @brief 创建一个新的表。
 *
 * @param tableManager 指向 TableManager 的指针。
 * @param tblname 表的名称。
 * @param sch 表的模式信息。
 * @param transaction 当前事务的指针。
 */
void TableManagerCreateTable(TableManager *tableManager, CString *tblname, Schema *sch, Transaction *transaction);

/**
 * @brief 初始化一个新的 TableManager 实例。
 *
 * @param isNew 是否是新创建的实例，如果是新创建的则为 true。
 * @param transaction 当前事务的指针。
 * @return 返回初始化后的 TableManager 指针。
 */
TableManager *TableManagerInit(bool isNew, Transaction *transaction);

/**
 * @brief 获取指定表的布局信息。
 *
 * @param tableManager 指向 TableManager 的指针。
 * @param tblname 表名称。
 * @param transaction 当前事务的指针。
 * @return 返回表的布局信息。
 */
Layout *TableManagerGetLayout(TableManager *tableManager, CString *tblname, Transaction *transaction);

#endif //DBMS_C_TABLEMANAGER_H
