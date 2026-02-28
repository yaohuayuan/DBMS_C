//
// Created by yaohuayuan on 2024/12/5.
//

#include "Planner.h"
#include "Parser.h"
Planner *PlannerInit(BasicQueryPlanner*queryPlanner,BasicUpdatePlanner *updatePlanner){
    Planner *planner = malloc(sizeof(Planner));
    planner->updatePlanner = updatePlanner;
    planner->queryPlanner = queryPlanner;
    return planner;
}
Plan* PlannerCreateQueryPlan(Planner*planner,CString*qry,Transaction*transaction){
    Parser *parser = ParserInit(CStringGetPtr(qry));
    QueryData*data = ParserQuery(parser);
    return BasicQueryPlannerCreatPlan(planner->queryPlanner,data,transaction);
}
int PlannerExecuteUpdate(Planner*planner,CString *cmd,Transaction*transaction){
    Parser*parser = ParserInit(CStringGetPtr(cmd));
    CommandData *commandData =   ParserUpdateCmd(parser);
    CommandCode commandCode = commandData->code;
    if(commandCode==CMD_INSERT_DATA){
        return BasicUpdatePlannerExecuteInsert(planner->updatePlanner,commandData->data.insertData,transaction);
    }else if(commandCode==CMD_DELETE_DATA){
        return BasicUpdatePlannerExecuteDelete(planner->updatePlanner,commandData->data.deleteData,transaction);
    }else if(commandCode == CMD_CREATE_INDEX){
        return BasicUpdatePlannerExecuteCreateIndex(planner->updatePlanner,commandData->data.createIndexData,transaction);
    }else if(commandCode == CMD_MODIFY_DATA){
        return BasicUpdatePlannerExecuteModify(planner->updatePlanner,commandData->data.modifyData,transaction);
    } else if(commandCode == CMD_CREATE_TABLE){
        return BasicUpdatePlannerExecuteCreateTable(planner->updatePlanner,commandData->data.createTableData,transaction);
    } else if(commandCode == CMD_CREATE_VIEW){
        return BasicUpdatePlannerExecuteCreateView(planner->updatePlanner,commandData->data.createViewData,transaction);
    }
    return 0;
}