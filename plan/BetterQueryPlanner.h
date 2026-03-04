//
// Created by yaohuayuan on 2024/12/5.
//

#ifndef DBMS_C_BETTERQUERYPLANNER_H
#define DBMS_C_BETTERQUERYPLANNER_H
#include "MetadataManager.h"
#include "QueryData.h"
#include "Plan.h"
#include "List.h"
#include "Transaction.h"
/**
 * @brief BasicQueryPlanner 结构体定义，用于实现基本的查询计划生成器。
 */
typedef struct BetterQueryPlanner{
    MetadataMgr *metadataMgr; ///< 指向元数据管理器的指针
}BetterQueryPlanner;

/**
 * @brief 初始化一个新的 BasicQueryPlanner 实例。
 *
 * @param metadataMgr 指向元数据管理器的指针。
 * @return 返回初始化后的 BasicQueryPlanner 指针。
 */
BetterQueryPlanner *BetterQueryPlannerInit(MetadataMgr*metadataMgr);

/**
 * @brief 创建查询计划。
 *
 * @param basicQueryPlanner 指向 BasicQueryPlanner 的指针。
 * @param queryData 指向查询数据的指针。
 * @param transaction 指向当前事务的指针。
 * @return 返回创建的查询计划指针。
 */
Plan *betterQueryPlannerCreatPlan(BetterQueryPlanner*basicQueryPlanner,QueryData*queryData,Transaction*transaction);

/**
 * @brief 估算连接成本。
 *
 * @param left 左表计划。
 * @param right 右表计划。
 * @param joinTerms 连接条件列表。
 * @return 返回连接成本。
 */
int EstimateJoinCost(Plan *left, Plan *right, List *joinTerms);

/**
 * @brief 移除记录数最小的计划。
 *
 * @param plans 计划列表。
 * @return 返回移除的计划。
 */
Plan* RemoveSmallestRecordsPlan(List *plans);

/**
 * @brief 贪心连接算法。
 *
 * @param basePlans 基础计划列表。
 * @param joinTerms 连接条件列表。
 * @return 返回连接后的计划。
 */
Plan* GreedyJoin(List *basePlans, List *joinTerms);
#endif //DBMS_C_BETTERQUERYPLANNER_H
