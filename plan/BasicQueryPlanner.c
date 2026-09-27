//
// Created by yaohuayuan on 2024/12/5.
//

#include "BasicQueryPlanner.h"
#include "Parser.h"
#include "Plan.h"
#include "CString.h"
#include "../trace/DBTrace.h"

BasicQueryPlanner *BasicQueryPlannerInit(MetadataMgr*metadataMgr){
    BasicQueryPlanner *basicQueryPlanner = malloc(sizeof (BasicQueryPlanner));
    basicQueryPlanner->metadataMgr = metadataMgr;
    return basicQueryPlanner;
}
Plan *BasicQueryPlannerCreatPlan(BasicQueryPlanner*basicQueryPlanner,QueryData*queryData,Transaction*transaction){
    CList *plans = CListInit(NULL, NULL, NULL);
    CListNode *tables = queryData->tables->head;
    while(tables){
        CString *tblName = ((CString *)tables->data);
        CString *viewDef = MetadataMgrGetViewDef(basicQueryPlanner->metadataMgr,tblName,transaction);
        if(viewDef!=NULL){
            Parser *parser = ParserInit(CStringGetPtr(viewDef));
            QueryData *view = ParserQuery(parser);
            Plan *plan = BasicQueryPlannerCreatPlan(basicQueryPlanner,view,transaction);
            CListAppend(plans,plan);
        }else{
            Plan *plan = PlanInit(TablePlanInit(transaction,tblName,basicQueryPlanner->metadataMgr),PLAN_TABLE_CODE);
            CListAppend(plans,plan);
        }
        tables = tables->next;
    }
    Plan *p = CListRemoveByIndex(plans,0);
    CListNode *head = plans->head;
    while(head){
        Plan*nextPlan = ((Plan *)head->data);
        ProductPlan *productPlan = ProductPlanInit(p,nextPlan);
        p = PlanInit(productPlan,PLAN_PRODUCT_CODE);
        head=head->next;
    }
    SelectPlan*selectPlan = SelectPlanInit(p,queryData->predicate);
    p = PlanInit(selectPlan,PLAN_SELECT_CODE);
    // ⭐ SELECT * 展开
    if (queryData->fields->size == 1) {
        CString *fld = (CString *)queryData->fields->head->data;

        if (strcmp(CStringGetPtr(fld), "*") == 0) {

            Schema *schema = PlanSchema(p);

 // 🔥 用 Schema 的字段替换 *
            CList *realFields = SchemaGetAllFields(schema);

            queryData->fields = realFields;
        }
    }
    ProjectPlan*projectPlan = ProjectPlanInit(p,queryData->fields);
    p = PlanInit(projectPlan,PLAN_PROJECT_CODE);
    PlanTraceTree(p);
    return p;
}
