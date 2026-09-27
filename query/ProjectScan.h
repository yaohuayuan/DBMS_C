//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_PROJECTSCAN_H
#define DBMS_C_PROJECTSCAN_H


#include "Scan.h"
#include "CList.h"
typedef struct ProjectScan{
    Scan *s;
    CList *fieldList;
}ProjectScan;

ProjectScan* ProjectScanInit(Scan *s,CList*fieldList);
void ProjectScanBeforeFirst(void *data);
bool ProjectScanNext(void *data);
bool ProjectScanHasField(void*data,CString *fldname);
int ProjectScanGetInt(void *data,CString *fldname);
const char * ProjectScanGetString(void *data,CString *fldname);
Constant * ProjectScanGetVal(void *data,CString *fldname);
void ProjectClose(void *data);

#endif //DBMS_C_PROJECTSCAN_H

