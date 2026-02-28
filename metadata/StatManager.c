//
// Created by yaohuayuan on 2024/11/19.
//

#include <stdlib.h>
#include <string.h>
#include "StatManager.h"


// 初始化StartManager
StatManager* StartManagerInit(TableManager *tblMgr, Transaction *tx) {
    StatManager *sm = (StatManager*)malloc(sizeof(StatManager));
    sm->tblMgr = tblMgr;
    sm->tablestats = malloc(sizeof(map_StartInfo_t));
    map_init(sm->tablestats);
    sm->numcalls = 0;
    StartManagerRefreshStatistics(sm, tx);
    return sm;
}

// 获取表的统计信息
StatInfo* StartManagerGetStatInfo(StatManager *sm, char *tblname, Layout *layout, Transaction *tx) {
    sm->numcalls++;
    if (sm->numcalls > 100) {
        StartManagerRefreshStatistics(sm, tx);
    }
    StatInfo *si = NULL;
    si=map_get(sm->tablestats, tblname);
    if (si== NULL) {
        si = StartManagerCalcTableStats(sm, tblname, layout, tx);
        map_set(sm->tablestats, tblname, *si);
    }
    return si;
}

// 刷新所有表的统计信息
void StartManagerRefreshStatistics(StatManager *sm, Transaction *tx) {
    map_deinit(sm->tablestats);
    sm->tablestats = malloc(sizeof(map_StartInfo_t));
    map_init(sm->tablestats);
    sm->numcalls = 0;
    CString *tblcatParam = CStringCreateFromCStr("tblcat");
    CString *tblnameParam = CStringCreateFromCStr("tblname");
    
    Layout *tcatlayout = TableManagerGetLayout(sm->tblMgr, tblcatParam, tx);
    TableScan *t = TableScanInit(tx, tblcatParam, tcatlayout);
    Scan*tcat = ScanInit(t,SCAN_TABLE_CODE);
    while (TableScanNext(tcat)) {
        char *tblnameStr = TableScanGetString(tcat, tblnameParam);
//        printf("%s\n",tblnameStr);
        CString *tblname = CStringCreateFromCStr(tblnameStr);
        Layout *layout = TableManagerGetLayout(sm->tblMgr, tblname, tx);
        StatInfo *si = StartManagerCalcTableStats(sm, tblnameStr, layout, tx);
        map_set(sm->tablestats, tblnameStr, *si);
        CStringDestroy(tblname);
    }
    TableScanClose(tcat);
    CStringDestroy(tblcatParam);
    CStringDestroy(tblnameParam);
}

// 计算单个表的统计信息
StatInfo* StartManagerCalcTableStats(StatManager *sm, char *tblname, Layout *layout, Transaction *tx) {
    int numRecs = 0;
    int numblocks = 0;
    CString *tblnameParam = CStringCreateFromCStr(tblname);
    TableScan *t = TableScanInit(tx, tblnameParam, layout);
    CStringDestroy(tblnameParam);
    Scan *ts = ScanInit(t,SCAN_TABLE_CODE);
    while (TableScanNext(ts)) {
        numRecs++;
        numblocks = TableScanGetRID(ts)->BlockNum + 1;
    }
    TableScanClose(ts);
    return StatInfoInit(numblocks, numRecs);
}

// 释放StartManager资源
void StartManagerFree(StatManager *sm) {
    map_deinit(sm->tablestats);
    free(sm);
}