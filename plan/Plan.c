//
// Created by yaohuayuan on 2024/11/30.
//

#include "Plan.h"
#include "TablePlan.h"
#include "ProductPlan.h"
#include "SelectPlan.h"
#include "ProjectPlan.h"

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

/**
 * @brief 检查计划是否包含指定的表。
 *
 * @param plan 指向 Plan 实例的指针。
 * @param tableName 表名。
 * @return 如果计划包含指定的表，返回 true；否则返回 false。
 */
bool PlanContainsTable(Plan *plan, CString *tableName) {
    if (!plan || !tableName) {
        return false;
    }

    switch (plan->code) {
        case PLAN_TABLE_CODE: {
            // 表计划：直接比较表名
            TablePlan *tablePlan = plan->planUnion.tablePlan;
            if (tablePlan) {
                return CStringCompare(tablePlan->tblname, tableName) == 0;
            }
            return false;
        }
        
        case PLAN_SELECT_CODE: {
            // 选择计划：检查其子计划
            SelectPlan *selectPlan = plan->planUnion.selectPlan;
            if (selectPlan && selectPlan->p) {
                return PlanContainsTable(selectPlan->p, tableName);
            }
            return false;
        }
        
        case PLAN_PRODUCT_CODE: {
            // 乘积计划：检查左右两个子计划
            ProductPlan *productPlan = plan->planUnion.productPlan;
            if (productPlan) {
                return PlanContainsTable(productPlan->p1, tableName) || 
                       PlanContainsTable(productPlan->p2, tableName);
            }
            return false;
        }
        
        case PLAN_PROJECT_CODE: {
            // 投影计划：检查其子计划
            ProjectPlan *projectPlan = plan->planUnion.projectPlan;
            if (projectPlan && projectPlan->p) {
                return PlanContainsTable(projectPlan->p, tableName);
            }
            return false;
        }
        
        default:
            return false;
    }
}