//
// Created by yaohuayuan on 2024/12/5.
//

#ifndef DBMS_C_PROJECTPLAN_H
#define DBMS_C_PROJECTPLAN_H
#include "Plan.h"
/**
 * @brief ProjectPlan 结构体定义，用于表示投影查询计划。
 */
typedef struct ProjectPlan{
    Plan *p;        ///< 输入计划指针
    Schema *schema; ///< 输出模式信息
}ProjectPlan;

/**
 * @brief 初始化一个新的 ProjectPlan 实例。
 *
 * @param plan 输入计划指针。
 * @param fieldlist 投影的字段列表。
 * @return 返回初始化后的 ProjectPlan 指针。
 */
ProjectPlan *ProjectPlanInit(Plan*plan,List*fieldlist);

/**
 * @brief 打开投影计划的扫描器。
 *
 * @param data 指向 ProjectPlan 实例的指针。
 * @return 返回创建的 Scan 指针。
 */
Scan* ProjectPlanOpen(void *data);

/**
 * @brief 计算投影计划访问的块数。
 *
 * @param data 指向 ProjectPlan 实例的指针。
 * @return 返回访问的块数。
 */
int ProjectPlanBlocksAccessed(void *data);

/**
 * @brief 计算投影计划输出的记录数。
 *
 * @param data 指向 ProjectPlan 实例的指针。
 * @return 返回输出的记录数。
 */
int ProjectPlanRecordsOutput(void *data);

/**
 * @brief 计算投影计划中指定字段的不同值数量。
 *
 * @param data 指向 ProjectPlan 实例的指针。
 * @param fldname 字段名称。
 * @return 返回不同值的数量。
 */
int ProjectPlanDistinctValue(void *data,CString *fldname);

/**
 * @brief 获取投影计划的输出模式。
 *
 * @param data 指向 ProjectPlan 实例的指针。
 * @return 返回输出模式信息。
 */
Schema *ProjectPlanSchema(void *data);
#endif //DBMS_C_PROJECTPLAN_H
