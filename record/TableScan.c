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
    BlockID *blockId = TransactionAppend(tableScan->transaction, tableScan->fileName );
    tableScan->recordPage = RecordPageInit(tableScan->transaction,blockId, tableScan->layout);
    RecordPageFormat(tableScan->recordPage);
    tableScan->currentSlot = -1;
}
void TableScanMoveToBlock(TableScan*tableScan,int blkNum){
    Scan *scan = ScanInit(tableScan,SCAN_TABLE_CODE);
    TableScanClose(scan);
    BlockID *blockId = BlockIDInit(tableScan->fileName ,blkNum);
    tableScan->recordPage = RecordPageInit(tableScan->transaction,blockId, tableScan->layout);
    tableScan->currentSlot = -1;

}
TableScan *TableScanInit(Transaction*transaction,CString*TBName,Layout*layout){
    TableScan *tableScan = malloc(sizeof (TableScan));
    tableScan->transaction = transaction;
    tableScan->layout = layout;
    
    // 构建文件名: TBName.tbl
    CString *filename = CStringCreateFromCString(TBName);
    CStringAppendCStr(filename, ".tbl");
    tableScan->fileName = filename;
    
    tableScan->recordPage = NULL;
    if(TransactionSize(transaction, filename )==0){
        TableScanMoveToNewBlock(tableScan);
    }else{
        TableScanMoveToBlock(tableScan,0);
    }
    return tableScan;
}
void TableScanBeforeFirst(void *data){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    TableScanMoveToBlock(tableScan,0);
}
bool TableScanAtLastBlock(TableScan *tableScan){
    return tableScan->recordPage->blockId->BlockID == TransactionSize(tableScan->transaction, tableScan->fileName )-1;
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
    BlockID *blockId= BlockIDInit(tableScan->fileName, rid->BlockNum);
    tableScan->recordPage = RecordPageInit(tableScan->transaction,blockId,tableScan->layout);
    tableScan->currentSlot = rid->Slot;
}
bool TableScanHasField(void *data,CString*fileName){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    return SchemaHasField(tableScan->layout->schema,fileName);
}
int TableScanGetInt(void *data,CString *fileName){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    return RecordPageGetInt(tableScan->recordPage,tableScan->currentSlot,fileName);
}
char* TableScanGetString(void *data,CString *fileName){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    return RecordPageGetString(tableScan->recordPage,tableScan->currentSlot,fileName);
}
void TableScanSetInt(void *data,CString*fileName,int idata){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    RecordSetInt(tableScan->recordPage,tableScan->currentSlot,fileName,idata);
}
void TableScanSetString(void *data,CString*fileName,CString* sdata){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    RecordSetString(tableScan->recordPage,tableScan->currentSlot,fileName,sdata);
}
RID* TableScanGetRID(void *data){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    return RIDInit(tableScan->recordPage->blockId->BlockID,tableScan->currentSlot);
}
Constant* TableScanGetVal(void *data,CString *fldname){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    if(SchemaType(tableScan->layout->schema,fldname)==FILE_INFO_CODE_INTEGER){
        return ConstantInitInt(TableScanGetInt(scan,fldname));
    }else{
        return ConstantInitString(TableScanGetString(scan,fldname));
    }
}
void TableScanSetVal(void *data,CString *fldname,Constant*val){
    Scan*scan    = (Scan*)data;
    TableScan *tableScan = scan->scanUnion.tableScan;
    if(SchemaType(tableScan->layout->schema,fldname)==FILE_INFO_CODE_INTEGER){
        TableScanSetInt(scan,fldname,ConstantAsInt(val));
    }else{
        TableScanSetString(scan,fldname,ConstantAsCString(val));
    }
}