//
// Created by yaohuayuan on 2024/12/5.
//

#ifndef DBMS_C_SELECTPLAN_H
#define DBMS_C_SELECTPLAN_H
#include "Plan.h"
#include "Predicate.h"
typedef struct SelectPlan{
    Plan *p;
    Predicate *predicate;
}SelectPlan;
SelectPlan *SelectPlanInit(Plan*plan,Predicate*predicate);
Scan * SelectPlanOpen(void *data);
int SelectPlanBlocksAccessed(void *data);
int SelectPlanRecordsOutput(void *data);
int SelectPlanDistinctValues(void *data,char *fldname);
Schema *SelectPlanSchema(void *data);
#endif //DBMS_C_SELECTPLAN_H
