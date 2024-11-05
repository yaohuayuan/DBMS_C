//
// Created by yaohuayuan on 2024/11/4.
//

#ifndef DBMS_C_CONCURRENCYMANAGER_H
#define DBMS_C_CONCURRENCYMANAGER_H
#include "map.h"
#include "LockTable.h"
typedef struct ConCurrencyManager{
    LockTable *lockTable;
    map_str_t * mapStr;
}ConCurrencyManager;
#endif //DBMS_C_CONCURRENCYMANAGER_H
