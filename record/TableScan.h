//
// Created by yaohuayuan on 2024/11/15.
//

#ifndef DBMS_C_TABLESCAN_H
#define DBMS_C_TABLESCAN_H
#include "Transaction.h"
#include "Layout.h"
#include "RecordPage.h"
#include "RID.h"
#include "Constant.h"
#include "UpdateScan.h"
typedef struct TableScan{
    Transaction *transaction;
    Layout *layout;
    RecordPage *recordPage;
    char *fileName;
    int currentSlot;
//    UpdateScan *updateScan;
}TableScan;
void TableScanClose(void *data);
void TableScanMoveToNewBlock(TableScan*tableScan);
void TableScanMoveToBlock(TableScan*tableScan,int blkNum);
TableScan *TableScanInit(Transaction*transaction,char*TBName,Layout*layout);
void TableScanBeforeFirst(void *data);
bool TableScanAtLastBlock(TableScan *tableScan);
bool TableScanNext(void *data);
void TableScanInsert(void *data);
void TableScanDelete(void *data);
void TableScanMoveToRid(void *data,RID *rid);
bool TableScanHasField(void *data,char*fileNam);
int TableScanGetInt(void *data,char *fileName);
char* TableScanGetString(void *data,char *fileName);
void TableScanSetInt(void *data,char*fileName,int idata);
void TableScanSetString(void *data,char*fileName,char* sdata);
RID* TableScanGetRID(void *data);
Constant* TableScanGetVal(void *data,char *fldname);
void TableScanSetVal(void *data,char *fldname,Constant*val);
#endif //DBMS_C_TABLESCAN_H
