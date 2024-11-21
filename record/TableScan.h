//
// Created by yaohuayuan on 2024/11/15.
//

#ifndef DBMS_C_TABLESCAN_H
#define DBMS_C_TABLESCAN_H
#include "Transaction.h"
#include "Layout.h"
#include "RecordPage.h"
#include "RID.h"
typedef struct TableScan{
    Transaction *transaction;
    Layout *layout;
    RecordPage *recordPage;
    char *fileName;
    int currentSlot;
}TableScan;
void TableScanClose(TableScan *tableScan);
void TableScanMoveToNewBlock(TableScan*tableScan);
void TableScanMoveToBlock(TableScan*tableScan,int blkNum);
TableScan *TableScanInit(Transaction*transaction,char*TBName,Layout*layout);
void TableScanBeforeFirst(TableScan*tableScan);
bool TableScanAtLastBlock(TableScan *tableScan);
bool TableScanNext(TableScan*tableScan);
void TableScanInsert(TableScan*tableScan);
void TableScanDelete(TableScan*tableScan);
void TableMoveToRid(TableScan*tableScan,RID *rid);
bool TableScanHasField(TableScan*tableScan,char*fileNam);
int TableScanGetInt(TableScan *tableScan,char *fileName);
char* TableScanGetString(TableScan *tableScan,char *fileName);
void TableScanSetInt(TableScan*tableScan,char*fileName,int data);
void TableScanSetString(TableScan*tableScan,char*fileName,char* data);
RID* TableScanGetRID(TableScan*tableScan);
#endif //DBMS_C_TABLESCAN_H
