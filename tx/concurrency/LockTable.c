//
// Created by yaohuayuan on 2024/11/4.
//

#include "LockTable.h"
LockTable * LockTableInit(){
    LockTable *lockTable = malloc(sizeof(LockTable));
    lockTable->Locks = malloc(sizeof(map_int_t));
    map_init(lockTable->Locks);
    return lockTable;
}
int LockTableGetLocalVal(LockTable * lockTable,BlockID  blockId){
    char *key = BlockIDToString(blockId);
    int*value=map_get(lockTable->Locks,key);
    return value==NULL?0:*value;
}
bool LockTableWaitToLong(long long startTime){
    long nowTime = time(NULL);
    return nowTime-startTime>LockTableTime;
}
bool LockTableHasXLock(LockTable *lockTable,BlockID blockId){
    return LockTableGetLocalVal(lockTable,blockId)<0;
}
bool LockTableHasSOtherLock(LockTable *lockTable,BlockID blockId){
    return LockTableGetLocalVal(lockTable,blockId)>1;
}
void LockTableXLock(LockTable*lockTable,BlockID blockId,Error *error){
    int NowTime = time(NULL);
    while (LockTableHasSOtherLock(lockTable,blockId)&&!LockTableWaitToLong(NowTime)){
        // 待写(后期多线程会写)
    };
    if(LockTableHasSOtherLock(lockTable,blockId)){
        error->errorCode = Error_HasSOtherLock;
        error->reason = "LockTable has S Other Lock";
    }
    map_set(lockTable->Locks,BlockIDToString(blockId),-1);
}
void LockTableUnLock(LockTable*lockTable,BlockID blockId){
    int *val = map_get(lockTable->Locks,BlockIDToString(blockId));
    if(*val>1){
        map_set(lockTable->Locks,BlockIDToString(blockId),*val-1);
    }
    map_remove(lockTable->Locks,BlockIDToString(blockId));
}
void LockTableSLock(LockTable*lockTable,BlockID blockId,Error *error){
    int NowTime = time(NULL);
    while (LockTableHasXLock(lockTable,blockId)&&!LockTableWaitToLong(NowTime)){
        // 待写(后期多线程会写)
    };
    if(LockTableHasXLock(lockTable,blockId)){
        error->errorCode = Error_HasXLock;
        error->reason = "LockTable has X Lock";
    }
    int *val = map_get(lockTable->Locks,BlockIDToString(blockId));
    map_set(lockTable->Locks,BlockIDToString(blockId),*val+1);
}