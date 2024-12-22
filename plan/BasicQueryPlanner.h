//
// Created by yaohuayuan on 2024/12/5.
//

#ifndef DBMS_C_BASICQUERYPLANNER_H
#define DBMS_C_BASICQUERYPLANNER_H
#include "MetadataManager.h"
#include "QueryData.h"
typedef struct BasicQueryPlanner{
    MetadataMgr *metadataMgr;
}BasicQueryPlanner;
BasicQueryPlanner *BasicQueryPlannerInit(MetadataMgr*metadataMgr);
Plan *BasicQueryPlannerCreatPlan(BasicQueryPlanner*basicQueryPlanner,QueryData*queryData,Transaction*transaction);
#endif //DBMS_C_BASICQUERYPLANNER_H
