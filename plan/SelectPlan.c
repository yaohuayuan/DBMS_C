//
// Created by yaohuayuan on 2024/12/5.
//
#define MIN(A, B) ((A) < (B) ? (A) : (B))

#include "SelectPlan.h"
SelectPlan *SelectPlanInit(Plan*plan,Predicate*predicate){
    SelectPlan *selectPlan = malloc(sizeof(SelectPlan));
    selectPlan->predicate = predicate;
    selectPlan->p = plan;
    return selectPlan;
}
Scan * SelectPlanOpen(void *data){
    Plan*plan = (Plan*)data;
    SelectPlan *selectPlan = plan->planUnion.selectPlan;
    Scan*scan = selectPlan->p->open(selectPlan->p);
    SelectScan*selectScan = SelectScanInit(scan,selectPlan->predicate);
    Scan*scan1 = ScanInit(selectScan,SCAN_SELECT_CODE);
    return scan1;
}
int SelectPlanBlocksAccessed(void *data){
    Plan*plan = (Plan*)data;
    SelectPlan *selectPlan = plan->planUnion.selectPlan;
    return selectPlan->p->blocksAccessed(selectPlan->p);
}
int SelectPlanRecordsOutput(void *data){
    Plan*plan = (Plan*)data;
    SelectPlan *selectPlan = plan->planUnion.selectPlan;
    return selectPlan->p->recordsOutput(selectPlan->p) / PredicateReductionFactor(selectPlan->predicate,selectPlan->p);
}
int SelectPlanDistinctValues(void *data,char *fldname){
    Plan*plan = (Plan*)data;
    SelectPlan *selectPlan = plan->planUnion.selectPlan;
    if(PredicateEquatesWithConstant(selectPlan->predicate,fldname)!=NULL){
        return 1;
    }else{
        char *fldname2 = PredicateEquatesWithField(selectPlan->predicate,fldname);
        if(fldname2!=NULL){
            return MIN(selectPlan->p->distinctValues(selectPlan->p,fldname),selectPlan->p->distinctValues(selectPlan->p,fldname2));
        }else{
            return selectPlan->p->distinctValues(selectPlan->p,fldname);
        }
    }
}
Schema *SelectPlanSchema(void *data){
    Plan*plan = (Plan*)data;
    SelectPlan *selectPlan = plan->planUnion.selectPlan;
    return selectPlan->p->schema(selectPlan->p);
}