//
// Created by yaohuayuan on 2024/11/21.
//

#include "HashIndex.h"
HashIndex * HashIndexInit(Transaction*transaction,char *idxname,Layout*layout){
    HashIndex *hashIndex = malloc(sizeof(HashIndex));
    hashIndex->layout = layout;
    hashIndex->transaction = transaction;
    hashIndex->idxname = strdup(idxname);
    hashIndex->scan = NULL;
    hashIndex->constant = NULL;
    return hashIndex;
}
void HashIndexClose(HashIndex*hashIndex){
    if(hashIndex->scan!=NULL){
        TableScanClose(hashIndex->scan);
    }
}
void HashBeforeFirst(HashIndex*hashIndex,Constant *constant){
    HashIndexClose(hashIndex);
    hashIndex->constant = constant;
    int bucket = ConstantHashCode(constant)%HASH_INDEX_NUM_BUCKETS;
    char tblname[512];
    sprintf(tblname,"%s%d",hashIndex->idxname,bucket);
    TableScan*tableScan = TableScanInit(hashIndex->transaction,tblname,hashIndex->layout);
    hashIndex->scan = ScanInit(tableScan,SCAN_TABLE_CODE);
}
bool HashIndexNext(HashIndex*hashIndex){
    while(TableScanNext(hashIndex->scan)){
        if(ConstantEquals(TableScanGetVal(hashIndex->scan, "dataval"),hashIndex->constant))
            return true;
    }
    return false;
}
RID *HashIndexGetDataRID(HashIndex*hashIndex){
    int blknum = TableScanGetInt(hashIndex->scan,"block");
    int id = TableScanGetInt(hashIndex->scan,"id");
    return RIDInit(blknum,id);
}
void HashIndexInsert(HashIndex*hashIndex,Constant*val,RID*rid){
    HashBeforeFirst(hashIndex,val);
    TableScanInsert(hashIndex->scan);
    TableScanSetInt(hashIndex->scan,"block",rid->BlockNum);
    TableScanSetInt(hashIndex->scan,"id",rid->Slot);
    TableScanSetVal(hashIndex->scan,"dataval",val);
}
void HashIndexDelete(HashIndex *hashIndex,Constant *val,RID*rid){
    HashBeforeFirst(hashIndex,val);
    while (HashIndexNext(hashIndex)){
        if(RIDEqual(HashIndexGetDataRID(hashIndex),rid)){
            TableScanDelete(hashIndex->scan);
            return ;
        }
    }
}
int HashIndexSearchCost(int numblocks,int rpb){
    return numblocks/HASH_INDEX_NUM_BUCKETS;
}