//
// Created by yaohuayuan on 2024/12/5.
//

#ifndef DBMS_C_PRODUCTPLAN_H
#define DBMS_C_PRODUCTPLAN_H
#include "Plan.h"
/**
 * @brief ProductPlan 结构体定义，用于表示两个查询计划的乘积操作。
 */
typedef struct ProductPlan{
    Plan *p1,*p2;    ///< 两个输入计划指针
    Schema *schema;  ///< 输出模式信息
}ProductPlan;

/**
 * @brief 初始化一个新的 ProductPlan 实例。
 *
 * @param plan1 第一个输入计划指针。
 * @param plan2 第二个输入计划指针。
 * @return 返回初始化后的 ProductPlan 指针。
 */
ProductPlan *ProductPlanInit(Plan*plan1,Plan*plan2);

/**
 * @brief 打开乘积计划的扫描器。
 *
 * @param data 指向 ProductPlan 实例的指针。
 * @return 返回创建的 Scan 指针。
 */
Scan* ProductPlanOpen(void *data);

/**
 * @brief 计算乘积计划访问的块数。
 *
 * @param data 指向 ProductPlan 实例的指针。
 * @return 返回访问的块数。
 */
int ProductPlanBlocksAccessed(void *data);

/**
 * @brief 计算乘积计划输出的记录数。
 *
 * @param data 指向 ProductPlan 实例的指针。
 * @return 返回输出的记录数。
 */
int ProductPlanRecordsOutput(void *data);

/**
 * @brief 计算乘积计划中指定字段的不同值数量。
 *
 * @param data 指向 ProductPlan 实例的指针。
 * @param fldname 字段名称。
 * @return 返回不同值的数量。
 */
int ProductPlanDistinctValue(void *data,CString *fldname);

/**
 * @brief 获取乘积计划的输出模式。
 *
 * @param data 指向 ProductPlan 实例的指针。
 * @return 返回输出模式信息。
 */
Schema *ProductPlanSchema(void *data);
#endif //DBMS_C_PRODUCTPLAN_H
