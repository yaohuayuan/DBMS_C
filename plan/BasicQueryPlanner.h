//
// Created by yaohuayuan on 2024/12/5.
//

#ifndef DBMS_C_BASICQUERYPLANNER_H
#define DBMS_C_BASICQUERYPLANNER_H
#include "MetadataManager.h"
#include "QueryData.h"
/**
 * @brief BasicQueryPlanner 结构体定义，用于实现基本的查询计划生成器。
 */
typedef struct BasicQueryPlanner{
    MetadataMgr *metadataMgr; ///< 指向元数据管理器的指针
}BasicQueryPlanner;

/**
 * @brief 初始化一个新的 BasicQueryPlanner 实例。
 *
 * @param metadataMgr 指向元数据管理器的指针。
 * @return 返回初始化后的 BasicQueryPlanner 指针。
 */
BasicQueryPlanner *BasicQueryPlannerInit(MetadataMgr*metadataMgr);

/**
 * @brief 创建查询计划。
 *
 * @param basicQueryPlanner 指向 BasicQueryPlanner 的指针。
 * @param queryData 指向查询数据的指针。
 * @param transaction 指向当前事务的指针。
 * @return 返回创建的查询计划指针。
 */
Plan *BasicQueryPlannerCreatPlan(BasicQueryPlanner*basicQueryPlanner,QueryData*queryData,Transaction*transaction);
#endif //DBMS_C_BASICQUERYPLANNER_H
