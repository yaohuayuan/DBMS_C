//
// Created by yaohuayuan on 2024/12/5.
//

#ifndef DBMS_C_PROJECTPLAN_H
#define DBMS_C_PROJECTPLAN_H
#include "Plan.h"
typedef struct ProjectPlan{
    Plan *p;
    Schema *schema;
}ProjectPlan;
ProjectPlan *ProjectPlanInit(Plan*plan,List*fieldlist);
Scan* ProjectPlanOpen(void *data);
int ProjectPlanBlocksAccessed(void *data);
int ProjectPlanRecordsOutput(void *data);
int ProjectPlanDistinctValue(void *data,char *fldname);
Schema *ProjectPlanSchema(void *data);
#endif //DBMS_C_PROJECTPLAN_H
