//
// Created by yaohuayuan on 2024/11/30.
//

#ifndef DBMS_C_SELECTSCAN_H
#define DBMS_C_SELECTSCAN_H
#include "Scan.h"
#include "Predicate.h"
typedef struct Predicate Predicate;
typedef struct SelectScan{
    Scan *s;
    Predicate *predicate;
}SelectScan;
SelectScan *SelectScanInit(Scan*s,Predicate*predicate);
void SelectScanBeforeFirst(void*data);
bool SelectScanNext(void *data);
int SelectScanGetInt(void *data,CString *fldname);
char * SelectScanGetString(void *data,CString *fldname);
Constant *SelectScanGetVal(void *data,CString *fldname);
bool SelectScanHasField(void *data,CString *fldname);
void SelectScanClose(void *data);
void SelectScanSetInt(void *data,CString *fldname,int val);
void SelectScanSetString(void *data,CString *fldname,CString * val);
void SelectScanSetVal(void *data,CString *fldname,Constant *val);
void SelectScanDelete(void *data);
void SelectScanInsert(void *data);
RID* SelectScanGetRID(void *data);
void SelectScanMoveToRID(void *data,RID*rid);
#endif //DBMS_C_SELECTSCAN_H
