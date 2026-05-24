//
// Created by yaohuayuan on 2024/12/5.
//

#include "BasicUpdatePlanner.h"
#include "../trace/DBTrace.h"
BasicUpdatePlanner*  BasicUpdatePlannerInit(MetadataMgr*metadataMgr){
    BasicUpdatePlanner *basicUpdatePlanner = malloc(sizeof(BasicUpdatePlanner));
    basicUpdatePlanner->metadataMgr=metadataMgr;
    return basicUpdatePlanner;
}
int BasicUpdatePlannerExecuteDelete(BasicUpdatePlanner *basicUpdatePlanner,DeleteData*data,Transaction*transaction){
    UpdateExecutionTraceDelete(data);
    TablePlan *tablePlan = TablePlanInit(transaction,data->tblname,basicUpdatePlanner->metadataMgr);
    Plan *plan = PlanInit(tablePlan,PLAN_TABLE_CODE);
    SelectPlan *selectPlan = SelectPlanInit(plan,data->predicate);
    plan = PlanInit(selectPlan,PLAN_SELECT_CODE);
    Scan *scan = plan->open(plan);
    int count = 0;
    while (scan->next(scan)){
        scan->delete(scan);
        count++;
    }
    scan->close(scan);
    return count;
}
int BasicUpdatePlannerExecuteModify(BasicUpdatePlanner *basicUpdatePlanner,ModifyData *data,Transaction*transaction){
    UpdateExecutionTraceModify(data);
    TablePlan *tablePlan = TablePlanInit(transaction,data->tblname,basicUpdatePlanner->metadataMgr);
    Plan *plan = PlanInit(tablePlan,PLAN_TABLE_CODE);
    SelectPlan *selectPlan = SelectPlanInit(plan,data->predicate);
    plan = PlanInit(selectPlan,PLAN_SELECT_CODE);
    Scan *scan = plan->open(plan);
    int count = 0;
    while (scan->next(scan)){
        Constant *val = ExpressionEvaluate(data->newVal,scan);
        scan->setVal(scan,data->fldname,val);
        count++;
    }
    scan->close(scan);
    return count;
}
int BasicUpdatePlannerExecuteInsert(BasicUpdatePlanner *basicUpdatePlanner,InsertData *data,Transaction*transaction){
    UpdateExecutionTraceInsert(data);

    TablePlan *tablePlan = TablePlanInit(transaction,data->tblname,basicUpdatePlanner->metadataMgr);

    Plan *plan = PlanInit(tablePlan,PLAN_TABLE_CODE);
    Scan *scan = plan->open(plan);
    scan->insert(scan);
    CListNode *fldHead = data->flds->head;
    CListNode *valHead = data->vals->head;
    while (fldHead){
        Constant *val = ((Constant *)valHead->data);
        CString*fldname = ((CString *)fldHead->data);
        scan->setVal(scan,fldname,val);
        fldHead = fldHead->next;
        valHead = valHead->next;
    }
    scan->close(scan);
    return 1;
}
int BasicUpdatePlannerExecuteCreateTable(BasicUpdatePlanner *basicUpdatePlanner,CreateTableData *data,Transaction*transaction){
    UpdateExecutionTraceCreateTable(data);
    MetadataMgrCreateTable(basicUpdatePlanner->metadataMgr,data->tblname,data->schema,transaction);
    return 0;
}
int BasicUpdatePlannerExecuteCreateView(BasicUpdatePlanner *basicUpdatePlanner,CreateViewData *data,Transaction*transaction){
    UpdateExecutionTraceCreateView(data);
    char *viewDefStr = QueryDataToString(data->queryData);
    CString *viewDef = CStringCreateFromCStr(viewDefStr);
    MetadataMgrCreateView(basicUpdatePlanner->metadataMgr,data->viewName,viewDef,transaction);
    CStringDestroy(viewDef);
    return 0;
}
int BasicUpdatePlannerExecuteCreateIndex(BasicUpdatePlanner *basicUpdatePlanner,CreateIndexData *data,Transaction*transaction){
    UpdateExecutionTraceCreateIndex(data);
    MetadataMgrCreateIndex(basicUpdatePlanner->metadataMgr,data->idxname,data->tblname,data->fldname,transaction);
    return 0;
}
