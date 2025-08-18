//
// Created by yaohuayuan on 2024/11/4.
//

#ifndef DBMS_C_LOCKTABLE_H
#define DBMS_C_LOCKTABLE_H
#include "map.h"
#include "time.h"
#include "BlockId.h"
#include "Error.h"
#define LockTableTime 1
typedef struct LockTable{
    map_int_t *Locks;
}LockTable;
LockTable * LockTableInit();
int LockTableGetLocalVal(LockTable * lockTable,BlockID  *blockId);
bool LockTableWaitToLong(long long startTime);
bool LockTableHasXLock(LockTable *lockTable,BlockID *blockId);
bool LockTableHasSOtherLock(LockTable *lockTable,BlockID *blockId);
void LockTableXLock(LockTable*lockTable,BlockID *blockId,Error *error);
void LockTableUnLock(LockTable*lockTable,BlockID *blockId);
void LockTableSLock(LockTable*lockTable,BlockID *blockId,Error *error);
void LockTableUpgrade(LockTable *lockTable, BlockID *blockId, Error *error);
#endif //DBMS_C_LOCKTABLE_H
