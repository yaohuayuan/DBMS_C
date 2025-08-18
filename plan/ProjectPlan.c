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
        char *fldname = head->data->stringData;
        SchemaAdd(projectPlan->schema,fldname,projectPlan->p->schema(projectPlan->p));
        head=head->next;
    }
    return projectPlan;
}
Scan* ProjectPlanOpen(void *data){
    Plan*plan = (Plan*)data;
    ProjectPlan * projectPlan = plan->planUnion.projectPlan;
    Scan *s1 = projectPlan->p->open(projectPlan->p);
    List *head = ListInit(LIST_TYPE_STRING);
    Schema* schema = projectPlan->schema;
    FieldNode *current = schema->fields;  // 从头开始
    while (current) {
        ListAppend(head,current->fileName, strlen(current->fileName));
        current = current->next;
    }
    ProjectScan *projectScan = ProjectScanInit(s1,head);
    Scan *scan = ScanInit(projectScan,SCAN_PROJECT_CODE);
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
int ProjectPlanDistinctValue(void *data,char *fldname){
    Plan*plan = (Plan*)data;
    ProjectPlan * projectPlan = plan->planUnion.projectPlan;
    return projectPlan->p->distinctValues(projectPlan->p,fldname);
}
Schema *ProjectPlanSchema(void *data){
    Plan*plan = (Plan*)data;
    ProjectPlan * projectPlan = plan->planUnion.projectPlan;
    return projectPlan->schema;
}