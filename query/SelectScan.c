//
// Created by yaohuayuan on 2024/11/30.
//

#include "SelectScan.h"
SelectScan *SelectScanInit(Scan*s,Predicate*predicate){
    SelectScan *selectScan = malloc(sizeof(SelectScan));
    selectScan->s = s;
    selectScan->predicate = predicate;
    return selectScan;
}
void SelectScanBeforeFirst(void*data){
    Scan*scan = (Scan*)data;
    SelectScan *selectScan = scan->scanUnion.selectScan;
    selectScan->s->beforeFirst(selectScan->s);
}
bool SelectScanNext(void *data){
    Scan*scan = (Scan*)data;
    SelectScan *selectScan = scan->scanUnion.selectScan;
    while (selectScan->s->next(selectScan->s)){
        if(PredicateIsSatisfied(selectScan->predicate,selectScan->s)){
            return true;
        }
    }
    return false;
}
int SelectScanGetInt(void *data,char *fldname){
    Scan*scan = (Scan*)data;
    SelectScan *selectScan = scan->scanUnion.selectScan;
    return selectScan->s->getInt(selectScan->s,fldname);
}
char * SelectScanGetString(void *data,char *fldname){
    Scan*scan = (Scan*)data;
    SelectScan *selectScan = scan->scanUnion.selectScan;
    return selectScan->s->getString(selectScan->s,fldname);
}
Constant *SelectScanGetVal(void *data,char *fldname){
    Scan*scan = (Scan*)data;
    SelectScan *selectScan = scan->scanUnion.selectScan;
    return selectScan->s->getVal(selectScan->s,fldname);
}
bool SelectScanHasField(void *data,char *fldname){
    Scan*scan = (Scan*)data;
    SelectScan *selectScan = scan->scanUnion.selectScan;
    return selectScan->s->hasField(selectScan->s,fldname);
}
void SelectScanClose(void *data){
    Scan*scan = (Scan*)data;
    SelectScan *selectScan = scan->scanUnion.selectScan;
    selectScan->s->close(selectScan->s);
}
void SelectScanSetInt(void *data,char *fldname,int val){
    Scan*scan = (Scan*)data;
    SelectScan *selectScan = scan->scanUnion.selectScan;
    selectScan->s->setInt(selectScan->s,fldname,val);
}
void SelectScanSetString(void *data,char *fldname,char * val){
    Scan*scan = (Scan*)data;
    SelectScan *selectScan = scan->scanUnion.selectScan;
    selectScan->s->setString(selectScan->s,fldname,val);
}
void SelectScanSetVal(void *data,char *fldname,Constant *val){
    Scan*scan = (Scan*)data;
    SelectScan *selectScan = scan->scanUnion.selectScan;
    selectScan->s->setVal(selectScan->s,fldname,val);
}
void SelectScanDelete(void *data){
    Scan*scan = (Scan*)data;
    SelectScan *selectScan = scan->scanUnion.selectScan;
    selectScan->s->delete(selectScan->s);
}
void SelectScanInsert(void *data){
    Scan*scan = (Scan*)data;
    SelectScan *selectScan = scan->scanUnion.selectScan;
    selectScan->s->insert(selectScan->s);
}
RID* SelectScanGetRID(void *data){
    Scan*scan = (Scan*)data;
    SelectScan *selectScan = scan->scanUnion.selectScan;
    return selectScan->s->getRid(selectScan->s);
}
void SelectScanMoveToRID(void *data,RID*rid){
    Scan*scan = (Scan*)data;
    SelectScan *selectScan = scan->scanUnion.selectScan;
    selectScan->s->moveToRID(selectScan->s,rid);
}