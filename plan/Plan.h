//
// Created by yaohuayuan on 2024/11/30.
//

#ifndef DBMS_C_PLAN_H
#define DBMS_C_PLAN_H
#include "Scan.h"
#include "Schema.h"
#include "TablePlan.h"
#include "ProductPlan.h"
#include "SelectPlan.h"
#include "ProjectPlan.h"
#include "CString.h"
typedef struct ProjectPlan ProjectPlan;
typedef struct TablePlan TablePlan;
typedef struct ProductPlan ProductPlan;
typedef struct SelectPlan SelectPlan;
/**
 * @brief 计划类型枚举，定义了不同类型的查询计划。
 */
typedef enum PLAN_CODE{
    PLAN_SELECT_CODE,         ///< 选择计划类型
    PLAN_TABLE_CODE,          ///< 表计划类型
    PLAN_BASIC_QUERY_CODE,    ///< 基本查询计划类型
    PLAN_BASIC_UPDATE_CODE,   ///< 基本更新计划类型
    PLAN_OPTIMIZED_PRODUCT_CODE, ///< 优化后的乘积计划类型
    PLAN_PRODUCT_CODE,        ///< 乘积计划类型
    PLAN_PROJECT_CODE,        ///< 投影计划类型
}PLAN_CODE;

/**
 * @brief PlanUnion 联合类型，用于存储不同类型的计划数据。
 */
typedef union PlanUnion {
    ProjectPlan *projectPlan;   ///< 投影计划
    TablePlan *tablePlan;       ///< 表计划
    ProductPlan* productPlan;   ///< 乘积计划
    SelectPlan* selectPlan;     ///< 选择计划
} PlanUnion;

/**
 * @brief Plan 结构体定义，用于表示查询计划。
 */
typedef struct Plan{
    Scan *(*open)(void *plan);             ///< 打开扫描器的函数指针
    int (*blocksAccessed)(void *plan);     ///< 计算访问块数的函数指针
    int (*recordsOutput)(void *data);      ///< 计算输出记录数的函数指针
    int (*distinctValues)(void *data,CString *fldname); ///< 计算不同值数量的函数指针
    Schema *(*schema)(void *data);         ///< 获取模式信息的函数指针

    PLAN_CODE code;         ///< 计划类型
    PlanUnion planUnion;    ///< 计划数据联合
}Plan;

/**
 * @brief 初始化一个新的 Plan 实例。
 *
 * @param data 计划数据指针。
 * @param code 计划类型。
 * @return 返回初始化后的 Plan 指针。
 */
Plan *PlanInit(void *data,PLAN_CODE code);
#endif //DBMS_C_PLAN_H
