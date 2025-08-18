//
// Created by yaohuayuan on 2024/11/15.
//

#include "TableScan.h"
void TableScanClose(void *data){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    if(tableScan->recordPage!=NULL){
        TransactionUnPin(tableScan->transaction,tableScan->recordPage->blockId);
    }
}
void TableScanMoveToNewBlock(TableScan*tableScan){
    Scan *scan = ScanInit(tableScan,SCAN_TABLE_CODE);
    TableScanClose(scan);
    BlockID *blockId = TransactionAppend(tableScan->transaction, CStringCreateFromCStr(tableScan->fileName) );
    tableScan->recordPage = RecordPageInit(tableScan->transaction,blockId, tableScan->layout);
    RecordPageFormat(tableScan->recordPage);
    tableScan->currentSlot = -1;
}
void TableScanMoveToBlock(TableScan*tableScan,int blkNum){
    Scan *scan = ScanInit(tableScan,SCAN_TABLE_CODE);
    TableScanClose(scan);
    BlockID *blockId = BlockIDInit(CStringCreateFromCStr(tableScan->fileName) ,blkNum);
    tableScan->recordPage = RecordPageInit(tableScan->transaction,blockId, tableScan->layout);
    tableScan->currentSlot = -1;

}
TableScan *TableScanInit(Transaction*transaction,char*TBName,Layout*layout){
    TableScan *tableScan = malloc(sizeof (TableScan));
    tableScan->transaction = transaction;
    tableScan->layout = layout;
    char filename[512];
    sprintf(filename,"%s.tbl",TBName);
    tableScan->fileName = strdup(filename);
    tableScan->recordPage = NULL;
    if(TransactionSize(transaction, CStringCreateFromCStr(filename) )==0){
        TableScanMoveToNewBlock(tableScan);
    }else{
        TableScanMoveToBlock(tableScan,0);
    }
//    tableScan->updateScan->updateScanOps->ops->beforeFirst = TableScanBeforeFirst;
//    tableScan->updateScan->updateScanOps->ops->next = TableScanNext;
//    tableScan->updateScan->updateScanOps->ops->close = TableScanClose;
//    tableScan->updateScan->updateScanOps->ops->getInt = TableScanGetInt;
//    tableScan->updateScan->updateScanOps->ops->getString = TableScanGetString;
//    tableScan->updateScan->updateScanOps->ops->getInt = TableScanGetInt;
//    tableScan->updateScan->updateScanOps->ops->getVal = TableScanGetVal;
//    tableScan->updateScan->updateScanOps->ops->hasField = TableScanHasField;
//    tableScan->updateScan->updateScanOps->delete = TableScanDelete;
//    tableScan->updateScan->updateScanOps->insert = TableScanInsert;
//    tableScan->updateScan->updateScanOps->getRid = TableScanGetRID;
//    tableScan->updateScan->updateScanOps->moveToRID = TableScanMoveToRid;
//    tableScan->updateScan->updateScanOps->setInt = TableScanSetInt;
//    tableScan->updateScan->updateScanOps->setString = TableScanSetString;
//    tableScan->updateScan->updateScanOps->setVal = TableScanSetVal;

    return tableScan;
}
void TableScanBeforeFirst(void *data){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    TableScanMoveToBlock(tableScan,0);
}
bool TableScanAtLastBlock(TableScan *tableScan){
    return tableScan->recordPage->blockId->BlockID == TransactionSize(tableScan->transaction, CStringCreateFromCStr(tableScan->fileName) )-1;
}
bool TableScanNext(void *data) {
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
   // printf("Current slot before: %d\n", tableScan->currentSlot);
    tableScan->currentSlot = RecordPageNextAfter(tableScan->recordPage, tableScan->currentSlot);
   // printf("Next slot: %d\n", tableScan->currentSlot);
    while (tableScan->currentSlot < 0) {
        //printf("Moving to next block...\n");
        if (TableScanAtLastBlock(tableScan)) {
           // printf("At last block, exiting.\n");
            return false;
        }
        TableScanMoveToBlock(tableScan, tableScan->recordPage->blockId->BlockID + 1);
        tableScan->currentSlot = RecordPageNextAfter(tableScan->recordPage, tableScan->currentSlot);
        //printf("Slot after moving to next block: %d\n", tableScan->currentSlot);
    }
    return true;
}

void TableScanInsert(void *data){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    tableScan->currentSlot = RecordPageInsertAfter(tableScan->recordPage,tableScan->currentSlot);
    while (tableScan->currentSlot<0){
        if(TableScanAtLastBlock(tableScan)){
            TableScanMoveToNewBlock(tableScan);
        }else{
            TableScanMoveToBlock(tableScan,tableScan->recordPage->blockId->BlockID+1);
        }
        tableScan->currentSlot = RecordPageInsertAfter(tableScan->recordPage,tableScan->currentSlot);
    }
}
void TableScanDelete(void *data){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    RecordPageDelete(tableScan->recordPage,tableScan->currentSlot);
}
void TableScanMoveToRid(void *data,RID *rid){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    TableScanClose(tableScan);
    BlockID *blockId= BlockIDInit(CStringCreateFromCStr(tableScan->fileName), rid->BlockNum);
    tableScan->recordPage = RecordPageInit(tableScan->transaction,blockId,tableScan->layout);
    tableScan->currentSlot = rid->Slot;
}
bool TableScanHasField(void *data,char*fileName){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    return SchemaHasField(tableScan->layout->schema,fileName);
}
int TableScanGetInt(void *data,char *fileName){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    return RecordPageGetInt(tableScan->recordPage,tableScan->currentSlot,fileName);
}
char* TableScanGetString(void *data,char *fileName){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    return RecordPageGetString(tableScan->recordPage,tableScan->currentSlot,fileName);
}
void TableScanSetInt(void *data,char*fileName,int idata){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    RecordSetInt(tableScan->recordPage,tableScan->currentSlot,fileName,idata);
}
void TableScanSetString(void *data,char*fileName,char* sdata){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    RecordSetString(tableScan->recordPage,tableScan->currentSlot,fileName,sdata);
}
RID* TableScanGetRID(void *data){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    return RIDInit(tableScan->recordPage->blockId->BlockID,tableScan->currentSlot);
}
Constant* TableScanGetVal(void *data,char *fldname){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    if(SchemaType(tableScan->layout->schema,fldname)==FILE_INFO_CODE_INTEGER){
        return ConstantInitInt(TableScanGetInt(scan,fldname));
    }else{
        return ConstantInitString(TableScanGetString(scan,fldname));
    }
}
void TableScanSetVal(void *data,char *fldname,Constant*val){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    if(SchemaType(tableScan->layout->schema,fldname)==FILE_INFO_CODE_INTEGER){
        TableScanSetInt(scan,fldname,ConstantAsInt(val));
    }else{
        TableScanSetString(scan,fldname,ConstantAsString(val));
    }
}