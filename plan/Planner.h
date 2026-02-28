//
// Created by yaohuayuan on 2024/12/5.
//

#ifndef DBMS_C_PLANNER_H
#define DBMS_C_PLANNER_H
#include "BasicQueryPlanner.h"
#include "BasicUpdatePlanner.h"

/**
 * @brief Planner 结构体定义，用于管理查询计划和更新计划。
 */
typedef struct Planner{
    BasicUpdatePlanner *updatePlanner; ///< 指向基本更新计划器的指针
    BasicQueryPlanner *queryPlanner;   ///< 指向基本查询计划器的指针
}Planner;

/**
 * @brief 初始化一个新的 Planner 实例。
 *
 * @param queryPlanner 指向基本查询计划器的指针。
 * @param updatePlanner 指向基本更新计划器的指针。
 * @return 返回初始化后的 Planner 指针。
 */
Planner *PlannerInit(BasicQueryPlanner*queryPlanner,BasicUpdatePlanner *updatePlanner);

/**
 * @brief 创建查询计划。
 *
 * @param planner 指向 Planner 的指针。
 * @param qry 查询语句字符串。
 * @param transaction 指向当前事务的指针。
 * @return 返回创建的查询计划指针。
 */
Plan* PlannerCreateQueryPlan(Planner*planner,CString*qry,Transaction*transaction);

/**
 * @brief 执行更新操作。
 *
 * @param planner 指向 Planner 的指针。
 * @param cmd 更新命令字符串。
 * @param transaction 指向当前事务的指针。
 * @return 返回执行结果。
 */
int PlannerExecuteUpdate(Planner*planner,CString *cmd,Transaction*transaction);
#endif //DBMS_C_PLANNER_H
