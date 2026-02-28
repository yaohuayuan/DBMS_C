//
// Created by yaohuayuan on 2024/12/5.
//

#ifndef DBMS_C_BASICUPDATEPLANNER_H
#define DBMS_C_BASICUPDATEPLANNER_H
#include "MetadataManager.h"
#include "DeleteData.h"
#include "Transaction.h"
#include "Plan.h"
#include "Parser.h"

/**
 * @brief BasicUpdatePlanner 结构体定义，用于实现基本的更新计划生成器。
 */
typedef struct BasicUpdatePlanner{
    MetadataMgr *metadataMgr; ///< 指向元数据管理器的指针
}BasicUpdatePlanner;

/**
 * @brief 初始化一个新的 BasicUpdatePlanner 实例。
 *
 * @param metadataMgr 指向元数据管理器的指针。
 * @return 返回初始化后的 BasicUpdatePlanner 指针。
 */
BasicUpdatePlanner*  BasicUpdatePlannerInit(MetadataMgr*metadataMgr);

/**
 * @brief 执行删除操作。
 *
 * @param basicUpdatePlanner 指向 BasicUpdatePlanner 的指针。
 * @param data 指向删除数据的指针。
 * @param transaction 指向当前事务的指针。
 * @return 返回执行结果。
 */
int BasicUpdatePlannerExecuteDelete(BasicUpdatePlanner *basicUpdatePlanner,DeleteData*data,Transaction*transaction);

/**
 * @brief 执行修改操作。
 *
 * @param basicUpdatePlanner 指向 BasicUpdatePlanner 的指针。
 * @param data 指向修改数据的指针。
 * @param transaction 指向当前事务的指针。
 * @return 返回执行结果。
 */
int BasicUpdatePlannerExecuteModify(BasicUpdatePlanner *basicUpdatePlanner,ModifyData *data,Transaction*transaction);

/**
 * @brief 执行插入操作。
 *
 * @param basicUpdatePlanner 指向 BasicUpdatePlanner 的指针。
 * @param data 指向插入数据的指针。
 * @param transaction 指向当前事务的指针。
 * @return 返回执行结果。
 */
int BasicUpdatePlannerExecuteInsert(BasicUpdatePlanner *basicUpdatePlanner,InsertData *data,Transaction*transaction);

/**
 * @brief 执行创建表操作。
 *
 * @param basicUpdatePlanner 指向 BasicUpdatePlanner 的指针。
 * @param data 指向创建表数据的指针。
 * @param transaction 指向当前事务的指针。
 * @return 返回执行结果。
 */
int BasicUpdatePlannerExecuteCreateTable(BasicUpdatePlanner *basicUpdatePlanner,CreateTableData *data,Transaction*transaction);

/**
 * @brief 执行创建视图操作。
 *
 * @param basicUpdatePlanner 指向 BasicUpdatePlanner 的指针。
 * @param data 指向创建视图数据的指针。
 * @param transaction 指向当前事务的指针。
 * @return 返回执行结果。
 */
int BasicUpdatePlannerExecuteCreateView(BasicUpdatePlanner *basicUpdatePlanner,CreateViewData *data,Transaction*transaction);

/**
 * @brief 执行创建索引操作。
 *
 * @param basicUpdatePlanner 指向 BasicUpdatePlanner 的指针。
 * @param data 指向创建索引数据的指针。
 * @param transaction 指向当前事务的指针。
 * @return 返回执行结果。
 */
int BasicUpdatePlannerExecuteCreateIndex(BasicUpdatePlanner *basicUpdatePlanner,CreateIndexData *data,Transaction*transaction);
#endif //DBMS_C_BASICUPDATEPLANNER_H
