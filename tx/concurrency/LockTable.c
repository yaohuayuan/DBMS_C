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
int LockTableGetLocalVal(LockTable * lockTable,BlockID  *blockId){
    const char *key = CStringGetPtr(BlockID2CString(blockId)) ;
    int*value=map_get(lockTable->Locks,key);
    return value==NULL?0:*value;
}
bool LockTableWaitToLong(long long startTime){
    long nowTime = time(NULL);
    return nowTime-startTime>LockTableTime;
}
bool LockTableHasXLock(LockTable *lockTable,BlockID *blockId){
//    printf("%s has x :%d\n", CStringGetPtr(BlockID2CString(blockId)) ,LockTableGetLocalVal(lockTable,blockId));
    return LockTableGetLocalVal(lockTable,blockId)<0;
}
bool LockTableHasSOtherLock(LockTable *lockTable,BlockID *blockId){
    return LockTableGetLocalVal(lockTable,blockId)>1;
}
void LockTableXLock(LockTable*lockTable,BlockID *blockId,Error *error){
    int NowTime = time(NULL);
    while (LockTableHasSOtherLock(lockTable,blockId)&&!LockTableWaitToLong(NowTime)){
        printf("This has S Lock\n");
    };
    if(LockTableHasSOtherLock(lockTable,blockId)){
        error->errorCode = Error_HasSOtherLock;
        error->reason = "LockTable has S Other Lock";
    }
    map_set(lockTable->Locks, CStringGetPtr(BlockID2CString(blockId)),-1);
}
void LockTableUnLock(LockTable*lockTable,BlockID *blockId){
    int *val = map_get(lockTable->Locks,CStringGetPtr(BlockID2CString(blockId)));
    if(*val>1){
        map_set(lockTable->Locks,CStringGetPtr(BlockID2CString(blockId)),*val-1);
    }
    map_remove(lockTable->Locks,CStringGetPtr(BlockID2CString(blockId)));
}
void LockTableSLock(LockTable*lockTable,BlockID *blockId,Error *error){
    int NowTime = time(NULL);
    while (LockTableHasXLock(lockTable,blockId)&&!LockTableWaitToLong(NowTime)){
        printf("This has X Lock\n");
    };
    if(LockTableHasXLock(lockTable,blockId)){
        error->errorCode = Error_HasXLock;
        error->reason = "LockTable has X Lock";
    }
    int val = LockTableGetLocalVal(lockTable,blockId);
    map_set(lockTable->Locks,CStringGetPtr(BlockID2CString(blockId)),val+1);
}
void LockTableUpgrade(LockTable *lockTable, BlockID *blockId, Error *error) {
    int startTime = time(NULL);

    // 必须自己先持有 S 锁
    if (LockTableGetLocalVal(lockTable, blockId) <= 0) {
        error->errorCode = Error_NoSLockToUpgrade;
        error->reason = "No S lock held to upgrade";
        return;
    }

    // 等待其他事务释放 S 锁
    while (LockTableHasSOtherLock(lockTable, blockId) && !LockTableWaitToLong(startTime)) {
        printf("Waiting to upgrade S -> X...\n");
    }

    if (LockTableHasSOtherLock(lockTable, blockId)) {
        error->errorCode = Error_HasSOtherLock;
        error->reason = "Other transactions still hold S locks";
        return;
    }

    // 升级为 X 锁
    map_set(lockTable->Locks, CStringGetPtr(BlockID2CString(blockId)), -1);
}
