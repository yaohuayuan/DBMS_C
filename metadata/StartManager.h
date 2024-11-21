//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_STARTMANAGER_H
#define DBMS_C_STARTMANAGER_H

#include "TableManager.h"
#include "Transaction.h"
#include "StatInfo.h"
#include <stdbool.h>
#include "map.h"
typedef   map_t(StatInfo) map_StartInfo_t;
typedef struct {
    TableManager *tblMgr;
    map_StartInfo_t *tablestats;
    int numcalls;
} StartManager;

StartManager* StartManagerInit(TableManager *tblMgr, Transaction *tx);
StatInfo* StartManagerGetStatInfo(StartManager *sm, const char *tblname, Layout *layout, Transaction *tx);
void StartManagerRefreshStatistics(StartManager *sm, Transaction *tx);
StatInfo* StartManagerCalcTableStats(StartManager *sm, const char *tblname, Layout *layout, Transaction *tx);
void StartManagerFree(StartManager *sm);



#endif //DBMS_C_STARTMANAGER_H
