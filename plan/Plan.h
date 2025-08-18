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
typedef struct ProjectPlan ProjectPlan;
typedef struct TablePlan TablePlan;
typedef struct ProductPlan ProductPlan;
typedef struct SelectPlan SelectPlan;
typedef enum PLAN_CODE{
    PLAN_SELECT_CODE,
    PLAN_TABLE_CODE,
    PLAN_BASIC_QUERY_CODE,
    PLAN_BASIC_UPDATE_CODE,
    PLAN_OPTIMIZED_PRODUCT_CODE,
    PLAN_PRODUCT_CODE,
    PLAN_PROJECT_CODE,
}PLAN_CODE;
typedef union PlanUnion {
    ProjectPlan *projectPlan;
    TablePlan *tablePlan;
    ProductPlan* productPlan;
    SelectPlan* selectPlan;
} PlanUnion;
typedef struct Plan{
    Scan *(*open)(void *plan);
    int (*blocksAccessed)(void *plan);
    int (*recordsOutput)(void *data);
    int (*distinctValues)(void *data,char *fldname);
    Schema *(*schema)(void *data);

    PLAN_CODE code;
    PlanUnion planUnion;
}Plan;
Plan *PlanInit(void *data,PLAN_CODE code);
#endif //DBMS_C_PLAN_H
