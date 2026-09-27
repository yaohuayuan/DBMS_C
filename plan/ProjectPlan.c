//
// Created by yaohuayuan on 2024/12/5.
//

#include "ProjectPlan.h"
#include "ProjectScan.h"

static bool CStringListEquals(const void *a, const void *b) {
    return CStringEqual((const CString *)a, (const CString *)b) != 0;
}



ProjectPlan *ProjectPlanInit(Plan* plan, CList* fieldlist) {
    ProjectPlan *projectPlan = malloc(sizeof(ProjectPlan));
    projectPlan->p = plan;
    projectPlan->schema = SchemaInit();

    // ⭐ 先拿一次子计划的 Schema，避免在循环里递归调用
    Schema *inputSchema = plan->schema(plan);

    CListNode *head = fieldlist->head;
    while(head) {
        CString *fldname = ((CString *)head->data);

        // ⭐ 增加去重判断：如果 ProjectPlan 的 Schema 里已经有了这个字段，就不加了
        if (!SchemaHasField(projectPlan->schema, fldname)) {
            SchemaAdd(projectPlan->schema, fldname, inputSchema);
        }

        head = head->next;
    }
    return projectPlan;
}

Scan* ProjectPlanOpen(void *data){
    Plan*plan = (Plan*)data;
    ProjectPlan * projectPlan = plan->planUnion.projectPlan;
    Scan *s1 = projectPlan->p->open(projectPlan->p);

    CList *fieldlist = CListInit(NULL, CStringListEquals, NULL);
    Schema* schema = projectPlan->schema;
    FieldNode *current = schema->fields; // 从头开始
    while (current) {
        // 复制字段名，因为current->fileName可能会被后续操作修改或释放
        CString *fldname = CStringCreateFromCString(current->fileName);
        CListAppend(fieldlist, fldname);
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