//
// Created by yaohuayuan on 2024/11/15.
//

#include "TableScan.h"
void TableScanClose(TableScan *tableScan){
    if(tableScan->recordPage!=NULL){
        TransactionUnPin(tableScan->transaction,tableScan->recordPage->blockId);
    }
}
void TableScanMoveToNewBlock(TableScan*tableScan){
    TableScanClose(tableScan);
    BlockID blockId = TransactionAppend(tableScan->transaction,tableScan->fileName);
    tableScan->recordPage = RecordPageInit(tableScan->transaction,blockId, tableScan->layout);
    RecordPageFormat(tableScan->recordPage);
    tableScan->currentSlot = -1;
}
void TableScanMoveToBlock(TableScan*tableScan,int blkNum){
    TableScanClose(tableScan);
    BlockID blockId ;
    BlockID_Init(&blockId,tableScan->fileName,blkNum);
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
    if(TransactionSize(transaction,filename)==0){
        TableScanMoveToNewBlock(tableScan);
    }else{
        TableScanMoveToBlock(tableScan,0);
    }
    return tableScan;
}
void TableScanBeforeFirst(TableScan*tableScan){
    TableScanMoveToBlock(tableScan,0);
}
bool TableScanAtLastBlock(TableScan *tableScan){
    return tableScan->recordPage->blockId.blockId == TransactionSize(tableScan->transaction,tableScan->fileName)-1;
}
bool TableScanNext(TableScan*tableScan){
    tableScan->currentSlot = RecordPageNextAfter(tableScan->recordPage,tableScan->currentSlot);
    while(tableScan->currentSlot<0){
        if(TableScanAtLastBlock(tableScan)){
            return false;
        }
        TableScanMoveToBlock(tableScan,tableScan->recordPage->blockId.blockId+1);
        tableScan->currentSlot = RecordPageNextAfter(tableScan->recordPage,tableScan->currentSlot);
    }
    return true;
}
void TableScanInsert(TableScan*tableScan){
    tableScan->currentSlot = RecordPageInsertAfter(tableScan->recordPage,tableScan->currentSlot);
    while (tableScan->currentSlot<0){
        if(TableScanAtLastBlock(tableScan)){
            TableScanMoveToNewBlock(tableScan);
        }else{
            TableScanMoveToBlock(tableScan,tableScan->recordPage->blockId.blockId+1);
        }
        tableScan->currentSlot = RecordPageInsertAfter(tableScan->recordPage,tableScan->currentSlot);
    }
}
void TableScanDelete(TableScan*tableScan){
    RecordPageDelete(tableScan->recordPage,tableScan->currentSlot);
}
void TableMoveToRid(TableScan*tableScan,RID *rid){
    TableScanClose(tableScan);
    BlockID blockId;
    BlockID_Init(&blockId,tableScan->fileName,rid->BlockNum);
    tableScan->recordPage = RecordPageInit(tableScan->transaction,blockId,tableScan->layout);
    tableScan->currentSlot = rid->Slot;
}
bool TableScanHasField(TableScan*tableScan,char*fileName){
    return SchemaHasField(tableScan->layout->schema,fileName);
}
int TableScanGetInt(TableScan *tableScan,char *fileName){
    return RecordPageGetInt(tableScan->recordPage,tableScan->currentSlot,fileName);
}
char* TableScanGetString(TableScan *tableScan,char *fileName){
    return RecordPageGetString(tableScan->recordPage,tableScan->currentSlot,fileName);
}
void TableScanSetInt(TableScan*tableScan,char*fileName,int data){
    RecordSetInt(tableScan->recordPage,tableScan->currentSlot,fileName,data);
}
void TableScanSetString(TableScan*tableScan,char*fileName,char* data){
    RecordSetString(tableScan->recordPage,tableScan->currentSlot,fileName,data);
}
RID* TableScanGetRID(TableScan*tableScan){
    return RIDInit(tableScan->recordPage->blockId.blockId,tableScan->currentSlot);
}