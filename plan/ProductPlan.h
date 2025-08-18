//
// Created by yaohuayuan on 2024/12/5.
//

#ifndef DBMS_C_PRODUCTPLAN_H
#define DBMS_C_PRODUCTPLAN_H
#include "Plan.h"
typedef struct ProductPlan{
    Plan *p1,*p2;
    Schema *schema;
}ProductPlan;
ProductPlan *ProductPlanInit(Plan*plan1,Plan*plan2);
Scan* ProductPlanOpen(void *data);
int ProductPlanBlocksAccessed(void *data);
int ProductPlanRecordsOutput(void *data);
int ProductPlanDistinctValue(void *data,char *fldname);
Schema *ProductPlanSchema(void *data);
#endif //DBMS_C_PRODUCTPLAN_H
