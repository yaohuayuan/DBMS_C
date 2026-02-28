//
// Created by yaohuayuan on 2024/12/5.
//

#ifndef DBMS_C_SELECTPLAN_H
#define DBMS_C_SELECTPLAN_H
#include "Plan.h"
#include "Predicate.h"
/**
 * @brief SelectPlan 结构体定义，用于表示选择查询计划。
 */
typedef struct SelectPlan{
    Plan *p;              ///< 输入计划指针
    Predicate *predicate; ///< 选择条件
}SelectPlan;

/**
 * @brief 初始化一个新的 SelectPlan 实例。
 *
 * @param plan 输入计划指针。
 * @param predicate 选择条件。
 * @return 返回初始化后的 SelectPlan 指针。
 */
SelectPlan *SelectPlanInit(Plan*plan,Predicate*predicate);

/**
 * @brief 打开选择计划的扫描器。
 *
 * @param data 指向 SelectPlan 实例的指针。
 * @return 返回创建的 Scan 指针。
 */
Scan * SelectPlanOpen(void *data);

/**
 * @brief 计算选择计划访问的块数。
 *
 * @param data 指向 SelectPlan 实例的指针。
 * @return 返回访问的块数。
 */
int SelectPlanBlocksAccessed(void *data);

/**
 * @brief 计算选择计划输出的记录数。
 *
 * @param data 指向 SelectPlan 实例的指针。
 * @return 返回输出的记录数。
 */
int SelectPlanRecordsOutput(void *data);

/**
 * @brief 计算选择计划中指定字段的不同值数量。
 *
 * @param data 指向 SelectPlan 实例的指针。
 * @param fldname 字段名称。
 * @return 返回不同值的数量。
 */
int SelectPlanDistinctValues(void *data,CString *fldname);

/**
 * @brief 获取选择计划的输出模式。
 *
 * @param data 指向 SelectPlan 实例的指针。
 * @return 返回输出模式信息。
 */
Schema *SelectPlanSchema(void *data);
#endif //DBMS_C_SELECTPLAN_H
