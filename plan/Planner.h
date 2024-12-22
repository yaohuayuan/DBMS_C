//
// Created by yaohuayuan on 2024/12/5.
//

#ifndef DBMS_C_PLANNER_H
#define DBMS_C_PLANNER_H
#include "BasicQueryPlanner.h"
#include "BasicUpdatePlanner.h"

typedef struct Planner{
    BasicUpdatePlanner *updatePlanner;
    BasicQueryPlanner *queryPlanner;
}Planner;
Planner *PlannerInit(BasicQueryPlanner*queryPlanner,BasicUpdatePlanner *updatePlanner);
Plan* PlannerCreateQueryPlan(Planner*planner,char*qry,Transaction*transaction);
int PlannerExecuteUpdate(Planner*planner,char *cmd,Transaction*transaction);
#endif //DBMS_C_PLANNER_H
