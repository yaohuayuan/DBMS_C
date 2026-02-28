//
// Created by yaohuayuan on 2024/12/5.
//

#include "ProjectPlan.h"



ProjectPlan *ProjectPlanInit(Plan*plan,List*fieldlist){
    ProjectPlan *projectPlan = malloc(sizeof(ProjectPlan));
    projectPlan->p = plan;
    projectPlan->schema = SchemaInit();
    ListNode *head = fieldlist->head;
    while(head){
        CString *fldname = head->value.stringData;
        SchemaAdd(projectPlan->schema,fldname,projectPlan->p->schema(projectPlan->p));
        head=head->next;
    }
    return projectPlan;
}

Scan* ProjectPlanOpen(void *data){
    Plan*plan = (Plan*)data;
    ProjectPlan * projectPlan = plan->planUnion.projectPlan;
    Scan *s1 = projectPlan->p->open(projectPlan->p);
    
    List *fieldlist = ListInit(LIST_TYPE_STRING, NULL, NULL, NULL);
    Schema* schema = projectPlan->schema;
    FieldNode *current = schema->fields;  // 从头开始
    while (current) {
        // 复制字段名，因为current->fileName可能会被后续操作修改或释放
        CString *fldname = CStringCreateFromCString(current->fileName);
        ListAppend(fieldlist, fldname);
        current = current->next;
    }
    ProjectScan *projectScan = ProjectScanInit(s1, fieldlist);
    Scan *scan = ScanInit(projectScan, SCAN_PROJECT_CODE);
    return scan;
}
int ProjectPlanBlocksAccessed(void *data){
    Plan*plan = (Plan*)data;
    ProjectPlan * projectPlan = plan->planUnion.projectPlan;
    return projectPlan->p->blocksAccessed(projectPlan->p);
}
int ProjectPlanRecordsOutput(void *data){
    Plan*plan = (Plan*)data;
    ProjectPlan * projectPlan = plan->planUnion.projectPlan;
    return projectPlan->p->recordsOutput(projectPlan->p);
}
int ProjectPlanDistinctValue(void *data,CString *fldname){
    Plan*plan = (Plan*)data;
    ProjectPlan * projectPlan = plan->planUnion.projectPlan;
    return projectPlan->p->distinctValues(projectPlan->p,fldname);
}
Schema *ProjectPlanSchema(void *data){
    Plan*plan = (Plan*)data;
    ProjectPlan * projectPlan = plan->planUnion.projectPlan;
    return projectPlan->schema;
}