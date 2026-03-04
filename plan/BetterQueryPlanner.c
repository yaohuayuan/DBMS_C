//
// Created by yaohuayuan on 2024/12/5.
//

#include "BetterQueryPlanner.h"
#include "MetadataManager.h"
#include "Parser.h"
#include "Plan.h"
#include"CMap.h"
#define MAX(A,B) ((A) > (B) ? (A) : (B))

int EstimateJoinCost(
        Plan *left,
        Plan *right,
        List *joinTerms)
{
    int leftRows   = left->recordsOutput(left);
    int rightRows  = right->recordsOutput(right);
    int leftBlocks = left->blocksAccessed(left);
    int rightBlocks= right->blocksAccessed(right);

    int output = 0;
    bool hasJoin = false;

    ListNode *node = joinTerms->head;

    while (node) {
        Term *term = node->value.termData;

        CString *t1 = ExpressionGetTableName(term->lhs);
        CString *t2 = ExpressionGetTableName(term->rhs);

        if (t1 && t2) {

            bool leftHasT1  = PlanContainsTable(left,  t1);
            bool leftHasT2  = PlanContainsTable(left,  t2);
            bool rightHasT1 = PlanContainsTable(right, t1);
            bool rightHasT2 = PlanContainsTable(right, t2);

            // 如果是 left.right 的等值连接
            if ((leftHasT1 && rightHasT2) ||
                (leftHasT2 && rightHasT1)) {

                hasJoin = true;

                CString *leftField  =  ExpressionAsCStringFieldName(term->lhs);;
                CString *rightField =  ExpressionAsCStringFieldName(term->rhs);;

                int leftDistinct  = left->distinctValues(left, leftField);
                int rightDistinct = right->distinctValues(right, rightField);

                int denom = MAX(leftDistinct, rightDistinct);
                if (denom <= 0) denom = 1;

                output = (leftRows * rightRows) / denom;
                break; // 第一版只用一个 join 条件
                }
        }

        node = node->next;
    }

    // 如果没有 join 条件 → 笛卡尔积
    if (!hasJoin) {
        output = leftRows * rightRows;
    }

    // 使用你现在的嵌套循环模型
    int cost =
            leftBlocks
            + leftRows * rightBlocks
            + output;

    return cost;
}
Plan* RemoveSmallestRecordsPlan(List *plans){
    if (plans == NULL || plans->head == NULL) return NULL;
    ListNode *cur = plans->head;
    int idx = 0,bestIdx=0;
    Plan *firstPlan = (Plan*)cur->value.raw;
    int minRecords = firstPlan->recordsOutput(firstPlan);
    while (cur != NULL){
        Plan *plan = (Plan*)cur->value.raw;
        int rec = plan->recordsOutput(plan);
        if (rec < minRecords){
            minRecords = rec;
            bestIdx = idx;
        }
            idx++;
        cur = cur->next;
    }
    Plan *result = (Plan*)ListRemoveByIndex(plans,bestIdx);
    return result;
}
Plan* GreedyJoin(List *basePlans, List *joinTerms){
    if (basePlans == NULL || basePlans->size == 0) return NULL;
    if (basePlans->size == 1) {
        return (Plan*)ListRemoveByIndex(basePlans, 0);
    }
    Plan *current = RemoveSmallestRecordsPlan(basePlans);

    while (basePlans->size > 0){
        ListNode *curr = basePlans->head;
        int bestCost = INT_MAX;
        int bestIndex = 0;
        int index = 0;
        while (curr != NULL){
            Plan *plan = (Plan*)curr->value.raw;
            int cost = EstimateJoinCost(current, plan, joinTerms);
            if (cost < bestCost){
                bestCost = cost;
                bestIndex = index;
            }
            curr = curr->next;
            index++;
        }
        Plan *next = (Plan*)ListRemoveByIndex(basePlans, bestIndex);
        ProductPlan *prod = ProductPlanInit(current, next);
        current = PlanInit(prod, PLAN_PRODUCT_CODE);
    }
    return current;
}
BetterQueryPlanner *BetterQueryPlannerInit(MetadataMgr*metadataMgr){
    BetterQueryPlanner *betterQueryPlanner = malloc(sizeof (BetterQueryPlanner));
    betterQueryPlanner->metadataMgr = metadataMgr;
    return betterQueryPlanner;
}
Plan *betterQueryPlannerCreatPlan(BetterQueryPlanner*betterQueryPlanner,QueryData*queryData,Transaction*transaction){
    List *plans = ListInit(LIST_TYPE_PLAN, NULL, NULL, NULL);
    List *joinTerms = ListInit(LIST_TYPE_TERM, NULL, NULL, NULL);
    ListNode *termHead = queryData->predicate->terms->head;
    CMap tablePredicates;
    int key = CMapInit(&tablePredicates,sizeof(CString*),sizeof(List*),CStringCompareVoid,CStringDestroyVoid,NULL,NULL,NULL);
    while (termHead) {
        Term *term = termHead->value.termData;
        CString *t1 = ExpressionGetTableName(term->lhs);
        CString *t2 = ExpressionGetTableName(term->rhs);
        CString *targetTable = NULL;

        if (t1 == NULL && t2 == NULL) {
            continue;
        } else if (t1 == NULL) {
            targetTable = t2;               // 只有右边有表名
        } else if (t2 == NULL) {
            targetTable = t1;               // 只有左边有表名
        } else if (CStringCompare(t1, t2) == 0) {
            targetTable = t1;                // 两边同表
        } else {
            ListAppend(joinTerms, term);     // 涉及不同表，作为连接条件
        }

        if (targetTable != NULL) {
            // 获取该表对应的 term 列表，若不存在则创建
            List *tableTerms = CMapFind(&tablePredicates, targetTable);
            if (tableTerms == NULL) {
                tableTerms = ListInit(LIST_TYPE_TERM, NULL, NULL, NULL);
                CMapInsert(&tablePredicates, targetTable, tableTerms);
            }
            ListAppend(tableTerms, term);
        }

        termHead = termHead->next;
    }

    List *basePlans = ListInit(LIST_TYPE_PLAN, NULL, NULL, NULL);

    ListNode *tables = queryData->tables->head;
    while(tables){
        Plan *tblPlan = NULL;
        CString *tblName = tables->value.stringData;
        CString *viewDef = MetadataMgrGetViewDef(betterQueryPlanner->metadataMgr,tblName,transaction);
        if(viewDef!=NULL){
            Parser *parser = ParserInit(CStringGetPtr(viewDef));
            QueryData *view = ParserQuery(parser);
            tblPlan = betterQueryPlannerCreatPlan(betterQueryPlanner,view,transaction);
        }else{
            TablePlan*table_plan = TablePlanInit(transaction, tblName, betterQueryPlanner->metadataMgr);
            tblPlan = PlanInit(table_plan,PLAN_TABLE_CODE);
        }
        List* tableTerms = CMapFind(&tablePredicates, tblName);
        if (tableTerms!=NULL&&tableTerms->size>0){
            ListNode *tableTerm = tableTerms->head;
            Predicate *singlePred  = PredicateInit(tableTerm->value.termData);
            tableTerm = tableTerm->next;
            while(tableTerm){
                Predicate *tablepredicate = PredicateInit(tableTerm->value.termData);
                PredicateConjoinWith(singlePred ,tablepredicate);
                tableTerm = tableTerm->next;
            }
            SelectPlan * selectPlan = SelectPlanInit(tblPlan,singlePred);
            tblPlan = PlanInit(selectPlan, PLAN_SELECT_CODE);
        }
        ListAppend(basePlans, tblPlan);
        tables = tables->next;
    }
    Plan *p = GreedyJoin(basePlans, joinTerms);
    if (joinTerms->size>0){
        Predicate *joinPredicate = PredicateInit(joinTerms->head->value.termData);
        ListNode *joinTermHead = joinTerms->head;
        joinTermHead = joinTermHead->next;
        while (joinTermHead){
            PredicateConjoinWith(joinPredicate,PredicateInit(joinTermHead->value.termData));
            joinTermHead = joinTermHead->next;
        }
        SelectPlan*selectPlan = SelectPlanInit(p,joinPredicate);
        p = PlanInit(selectPlan,PLAN_SELECT_CODE);
    }


    ProjectPlan*projectPlan = ProjectPlanInit(p,queryData->fields);
    p = PlanInit(projectPlan,PLAN_PROJECT_CODE);
    return p;
}