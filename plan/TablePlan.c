//
// Created by yaohuayuan on 2024/11/30.
//

#include "TablePlan.h"
#include "Scan.h"
Scan *TablePlanOpen(void *data){
    Plan *plan = (Plan*)data;
    TablePlan*tablePlan =plan->planUnion.tablePlan;
    Scan *scan = ScanInit(TableScanInit(tablePlan->tx,tablePlan->tblname,tablePlan->layout),SCAN_TABLE_CODE);
    return scan;
}
TablePlan *TablePlanInit(Transaction*tx,CString *tblname,MetadataMgr*metadataMgr){
    TablePlan *tablePlan = malloc(sizeof(TablePlan));
    tablePlan->tblname = CStringCreateFromCString(tblname);
    tablePlan->tx = tx;
    tablePlan->layout = MetadataMgrGetLayout(metadataMgr,tblname,tx);
    tablePlan->si = MetadataMgrGetStatInfo(metadataMgr,tblname,tablePlan->layout,tx);

    return tablePlan;
}
int TablePlanBlocksAccessed(void *data){
    Plan *plan = (Plan*)data;
    TablePlan*tablePlan =plan->planUnion.tablePlan;
    return StatInfoBlocksAccessed(tablePlan->si);
}
int TablePlanRecordsOutput(void *data){
    Plan *plan = (Plan*)data;
    TablePlan*tablePlan =plan->planUnion.tablePlan;
    return StatInfoRecordsOutput(tablePlan->si);
}
int TablePlanDistinctValues(void *data,CString *fldname){
    Plan *plan = (Plan*)data;
    TablePlan*tablePlan =plan->planUnion.tablePlan;
    return StatInfoDistinctValues(tablePlan->si,CStringGetPtr(fldname));
}
Schema *TablePlanSchema(void *data){
    Plan *plan = (Plan*)data;
    TablePlan*tablePlan =plan->planUnion.tablePlan;
    return tablePlan->layout->schema;
}