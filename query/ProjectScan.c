//
// Created by yaohuayuan on 2024/11/19.
//

#include "ProjectScan.h"
void ProjectScanBeforeFirst(void *data){
    Scan*scan = (Scan*)data;
    ProjectScan *projectScan = scan->scanUnion.projectScan;
    projectScan->s->beforeFirst(projectScan->s);
}
bool ProjectScanNext(void *data){
    Scan*scan = (Scan*)data;
    ProjectScan *projectScan = scan->scanUnion.projectScan;
    return projectScan->s->next(projectScan->s);
}
bool ProjectScanHasField(void*data,CString *fldname){
    Scan*scan = (Scan*)data;
    ProjectScan *projectScan = scan->scanUnion.projectScan;
    return ListContains(projectScan->fieldList,fldname);
}
int ProjectScanGetInt(void *data,CString *fldname){
    Scan*scan = (Scan*)data;
    ProjectScan *projectScan = scan->scanUnion.projectScan;
    if(ProjectScanHasField(data,fldname)){
        return projectScan->s->getInt(projectScan->s,fldname);
    }
    return 0;
}
char * ProjectScanGetString(void *data,CString *fldname){
    Scan*scan = (Scan*)data;
    ProjectScan *projectScan = scan->scanUnion.projectScan;
    if(ProjectScanHasField(data,fldname)){
        return projectScan->s->getString(projectScan->s,fldname);
    }
    return NULL;
}

Constant * ProjectScanGetVal(void *data,CString *fldname){
    Scan*scan = (Scan*)data;
    ProjectScan *projectScan = scan->scanUnion.projectScan;
    if(ProjectScanHasField(data,fldname)){
        return projectScan->s->getVal(projectScan->s,fldname);
    }
    return NULL;
}
void ProjectClose(void *data){
    Scan*scan = (Scan*)data;
    ProjectScan *projectScan = scan->scanUnion.projectScan;
    projectScan->s->close(projectScan->s);
}
ProjectScan* ProjectScanInit(Scan *s,List*fieldList){
    ProjectScan *projectScan = malloc(sizeof (ProjectScan));
    projectScan->fieldList = fieldList;
    projectScan->s = s;
//    projectScan->s->ops->getVal = ProjectScanGetVal;
//    projectScan->s->ops->hasField = ProjectScanHasField;
//    projectScan->s->ops->getString = ProjectScanGetString;
//    projectScan->s->ops->getInt = ProjectScanGetInt;
//    projectScan->s->ops->next = ProjectScanNext;
//    projectScan->s->ops->beforeFirst = ProjectScanBeforeFirst;
//    projectScan->s->ops->close = ProjectClose;
    return projectScan;
}