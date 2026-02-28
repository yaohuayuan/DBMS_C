//
// Created by yaohuayuan on 2024/11/30.
//

#ifndef DBMS_C_TABLEPLAN_H
#define DBMS_C_TABLEPLAN_H
#include "Plan.h"
#include "Transaction.h"
#include "Layout.h"
#include "StatInfo.h"
#include "MetadataManager.h"
#include "CString.h"
/**
 * @brief TablePlan 结构体定义，用于表示表查询计划。
 */
typedef struct TablePlan{
    CString *tblname;    ///< 表名
    Transaction *tx;  ///< 事务指针
    Layout *layout;   ///< 表布局
    StatInfo *si;     ///< 表统计信息
}TablePlan;

/**
 * @brief 打开表计划的扫描器。
 *
 * @param data 指向 TablePlan 实例的指针。
 * @return 返回创建的 Scan 指针。
 */
Scan *TablePlanOpen(void*data);

/**
 * @brief 初始化一个新的 TablePlan 实例。
 *
 * @param tx 事务指针。
 * @param tblname 表名。
 * @param metadataMgr 元数据管理器指针。
 * @return 返回初始化后的 TablePlan 指针。
 */
TablePlan *TablePlanInit(Transaction*tx,CString *tblname,MetadataMgr*metadataMgr);

/**
 * @brief 计算表计划访问的块数。
 *
 * @param data 指向 TablePlan 实例的指针。
 * @return 返回访问的块数。
 */
int TablePlanBlocksAccessed(void *data);

/**
 * @brief 计算表计划输出的记录数。
 *
 * @param data 指向 TablePlan 实例的指针。
 * @return 返回输出的记录数。
 */
int TablePlanRecordsOutput(void *data);

/**
 * @brief 计算表计划中指定字段的不同值数量。
 *
 * @param data 指向 TablePlan 实例的指针。
 * @param fldname 字段名称。
 * @return 返回不同值的数量。
 */
int TablePlanDistinctValues(void *data,CString *fldname);

/**
 * @brief 获取表计划的输出模式。
 *
 * @param data 指向 TablePlan 实例的指针。
 * @return 返回输出模式信息。
 */
Schema *TablePlanSchema(void *data);
#endif //DBMS_C_TABLEPLAN_H
