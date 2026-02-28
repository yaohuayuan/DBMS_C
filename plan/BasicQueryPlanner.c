//
// Created by yaohuayuan on 2024/12/5.
//

#include "BasicQueryPlanner.h"
#include "Parser.h"
#include "Plan.h"
#include "CString.h"

BasicQueryPlanner *BasicQueryPlannerInit(MetadataMgr*metadataMgr){
    BasicQueryPlanner *basicQueryPlanner = malloc(sizeof (BasicQueryPlanner));
    basicQueryPlanner->metadataMgr = metadataMgr;
    return basicQueryPlanner;
}
Plan *BasicQueryPlannerCreatPlan(BasicQueryPlanner*basicQueryPlanner,QueryData*queryData,Transaction*transaction){
    List *plans = ListInit(LIST_TYPE_PLAN, NULL, NULL, NULL);
    ListNode *tables = queryData->tables->head;
    while(tables){
        CString *tblName = tables->value.stringData;
        CString *viewDef = MetadataMgrGetViewDef(basicQueryPlanner->metadataMgr,tblName,transaction);
        if(viewDef!=NULL){
            Parser *parser = ParserInit(CStringGetPtr(viewDef));
            QueryData *view = ParserQuery(parser);
            Plan *plan = BasicQueryPlannerCreatPlan(basicQueryPlanner,view,transaction);
            ListAppend(plans,plan);
        }else{
            Plan *plan = PlanInit(TablePlanInit(transaction,tblName,basicQueryPlanner->metadataMgr),PLAN_TABLE_CODE);
            ListAppend(plans,plan);
        }
        tables = tables->next;
    }
    Plan *p = ListRemoveByIndex(plans,0);
    ListNode *head = plans->head;
    while(head){
        Plan*nextPlan = head->value.planData;
        ProductPlan *productPlan = ProductPlanInit(p,nextPlan);
        p = PlanInit(productPlan,PLAN_PRODUCT_CODE);
        head=head->next;
    }
    SelectPlan*selectPlan = SelectPlanInit(p,queryData->predicate);
    p = PlanInit(selectPlan,PLAN_SELECT_CODE);

    ProjectPlan*projectPlan = ProjectPlanInit(p,queryData->fields);
    p = PlanInit(projectPlan,PLAN_PROJECT_CODE);
    return p;
}