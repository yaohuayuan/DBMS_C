//
// Created by yaohuayuan on 2024/11/19.
//

#include <stdlib.h>
#include <string.h>
#include "StartManager.h"
#include <stdlib.h>
#include <string.h>

// 初始化StartManager
StartManager* StartManagerInit(TableManager *tblMgr, Transaction *tx) {
    StartManager *sm = (StartManager*)malloc(sizeof(StartManager));
    sm->tblMgr = tblMgr;
    sm->tablestats = malloc(sizeof(map_StartInfo_t));
    map_init(sm->tablestats);
    sm->numcalls = 0;
    StartManagerRefreshStatistics(sm, tx);
    return sm;
}

// 获取表的统计信息
StatInfo* StartManagerGetStatInfo(StartManager *sm, const char *tblname, Layout *layout, Transaction *tx) {
    sm->numcalls++;
    if (sm->numcalls > 100) {
        StartManagerRefreshStatistics(sm, tx);
    }
    StatInfo *si = NULL;
    map_get(sm->tablestats, tblname);
    if (sm->tablestats->ref != NULL) {
        si = (StatInfo*)sm->tablestats->ref;
    } else {
        si = StartManagerCalcTableStats(sm, tblname, layout, tx);
        map_set(sm->tablestats, tblname, *si);
    }
    return si;
}

// 刷新所有表的统计信息
void StartManagerRefreshStatistics(StartManager *sm, Transaction *tx) {
    map_deinit(sm->tablestats);
    sm->tablestats = malloc(sizeof(map_StartInfo_t));
    map_init(sm->tablestats);
    sm->numcalls = 0;
    Layout *tcatlayout = TableManagerGetLayout(sm->tblMgr, "tblcat", tx);
    TableScan *tcat = TableScanInit(tx, "tblcat", tcatlayout);
    while (TableScanNext(tcat)) {
        const char *tblname = TableScanGetString(tcat, "tblname");
        Layout *layout = TableManagerGetLayout(sm->tblMgr, tblname, tx);
        StatInfo *si = StartManagerCalcTableStats(sm, tblname, layout, tx);
        map_set(sm->tablestats, tblname, *si);
    }
    TableScanClose(tcat);
}

// 计算单个表的统计信息
StatInfo* StartManagerCalcTableStats(StartManager *sm, const char *tblname, Layout *layout, Transaction *tx) {
    int numRecs = 0;
    int numblocks = 0;
    TableScan *ts = TableScanInit(tx, tblname, layout);
    while (TableScanNext(ts)) {
        numRecs++;
        numblocks = TableScanGetRID(ts)->BlockNum + 1;
    }
    TableScanClose(ts);
    return StatInfoInit(numblocks, numRecs);
}

// 释放StartManager资源
void StartManagerFree(StartManager *sm) {
    map_deinit(sm->tablestats);
    free(sm);
}