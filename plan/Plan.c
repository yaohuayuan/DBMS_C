//
// Created by yaohuayuan on 2024/11/30.
//

#include "Plan.h"
#include "TablePlan.h"
Plan *PlanInit(void *data,PLAN_CODE code){
    Plan *plan = malloc(sizeof (Plan));
    if(code == PLAN_TABLE_CODE){
        plan->code = PLAN_TABLE_CODE;
        plan->planUnion.tablePlan = (TablePlan*)data;
        plan->schema = TablePlanSchema;
        plan->distinctValues = TablePlanDistinctValues;
        plan->blocksAccessed = TablePlanBlocksAccessed;
        plan->recordsOutput =    TablePlanRecordsOutput;
        plan->open = TablePlanOpen;
    }else if(code == PLAN_PRODUCT_CODE){
        plan->code = PLAN_PRODUCT_CODE;
        plan->planUnion.productPlan =(ProductPlan*) data;
        plan->schema = ProductPlanSchema;
        plan->distinctValues = ProductPlanDistinctValue;
        plan->blocksAccessed = ProductPlanBlocksAccessed;
        plan->recordsOutput =    ProductPlanRecordsOutput;
        plan->open = ProductPlanOpen;
    }else if(code==PLAN_SELECT_CODE){
        plan->code = PLAN_SELECT_CODE;
        plan->planUnion.selectPlan =(SelectPlan*) data;
        plan->schema = SelectPlanSchema;
        plan->distinctValues = SelectPlanDistinctValues;
        plan->blocksAccessed = SelectPlanBlocksAccessed;
        plan->recordsOutput =    SelectPlanRecordsOutput;
        plan->open = SelectPlanOpen;
    }else if(code == PLAN_PROJECT_CODE){
        plan->code = PLAN_PROJECT_CODE;
        plan->planUnion.projectPlan =(ProjectPlan *) data;
        plan->schema = ProjectPlanSchema;
        plan->distinctValues = ProjectPlanDistinctValue;
        plan->blocksAccessed = ProjectPlanBlocksAccessed;
        plan->recordsOutput =    ProjectPlanRecordsOutput;
        plan->open = ProjectPlanOpen;
    }else if(code == PLAN_BASIC_QUERY_CODE){
        plan->code = PLAN_BASIC_QUERY_CODE;
    }
    return plan;
}