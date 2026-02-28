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
    CString *tblnameCStr = CStringCreateFromCStr(tblname);
    TableScan*tableScan = TableScanInit(hashIndex->transaction,tblnameCStr,hashIndex->layout);
    CStringDestroy(tblnameCStr);
    hashIndex->scan = ScanInit(tableScan,SCAN_TABLE_CODE);
}
bool HashIndexNext(HashIndex*hashIndex){
    CString *datavalParam = CStringCreateFromCStr("dataval");
    while(TableScanNext(hashIndex->scan)){
        if(ConstantEquals(TableScanGetVal(hashIndex->scan, datavalParam),hashIndex->constant)){
            CStringDestroy(datavalParam);
            return true;
        }
    }
    CStringDestroy(datavalParam);
    return false;
}
RID *HashIndexGetDataRID(HashIndex*hashIndex){
    CString *blockParam = CStringCreateFromCStr("block");
    CString *idParam = CStringCreateFromCStr("id");
    int blknum = TableScanGetInt(hashIndex->scan,blockParam);
    int id = TableScanGetInt(hashIndex->scan,idParam);
    CStringDestroy(blockParam);
    CStringDestroy(idParam);
    return RIDInit(blknum,id);
}
void HashIndexInsert(HashIndex*hashIndex,Constant*val,RID*rid){
    HashBeforeFirst(hashIndex,val);
    TableScanInsert(hashIndex->scan);
    CString *blockParam = CStringCreateFromCStr("block");
    CString *idParam = CStringCreateFromCStr("id");
    CString *datavalParam = CStringCreateFromCStr("dataval");
    TableScanSetInt(hashIndex->scan,blockParam,rid->BlockNum);
    TableScanSetInt(hashIndex->scan,idParam,rid->Slot);
    TableScanSetVal(hashIndex->scan,datavalParam,val);
    CStringDestroy(blockParam);
    CStringDestroy(idParam);
    CStringDestroy(datavalParam);
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