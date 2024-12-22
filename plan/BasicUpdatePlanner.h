//
// Created by yaohuayuan on 2024/12/5.
//

#ifndef DBMS_C_BASICUPDATEPLANNER_H
#define DBMS_C_BASICUPDATEPLANNER_H
#include "MetadataManager.h"
#include "DeleteData.h"
#include "Transaction.h"
#include "Plan.h"
#include "Parser.h"

typedef struct BasicUpdatePlanner{
    MetadataMgr *metadataMgr;
}BasicUpdatePlanner;
BasicUpdatePlanner*  BasicUpdatePlannerInit(MetadataMgr*metadataMgr);
int BasicUpdatePlannerExecuteDelete(BasicUpdatePlanner *basicUpdatePlanner,DeleteData*data,Transaction*transaction);
int BasicUpdatePlannerExecuteModify(BasicUpdatePlanner *basicUpdatePlanner,ModifyData *data,Transaction*transaction);
int BasicUpdatePlannerExecuteInsert(BasicUpdatePlanner *basicUpdatePlanner,InsertData *data,Transaction*transaction);
int BasicUpdatePlannerExecuteCreateTable(BasicUpdatePlanner *basicUpdatePlanner,CreateTableData *data,Transaction*transaction);
int BasicUpdatePlannerExecuteCreateView(BasicUpdatePlanner *basicUpdatePlanner,CreateViewData *data,Transaction*transaction);
int BasicUpdatePlannerExecuteCreateIndex(BasicUpdatePlanner *basicUpdatePlanner,CreateIndexData *data,Transaction*transaction);
#endif //DBMS_C_BASICUPDATEPLANNER_H
