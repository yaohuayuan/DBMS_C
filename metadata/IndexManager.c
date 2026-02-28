//
// Created by yaohuayuan on 2024/11/21.
//

#include "IndexManager.h"
#include <stdlib.h>
#include <string.h>

// 初始化 IndexMgr
IndexManager* IndexMgrInit(bool isNew, TableManager *tableManager, StatManager *statManager, Transaction *tx) {
    IndexManager *indexMgr = (IndexManager*)malloc(sizeof(IndexManager));
    indexMgr->tableManager = tableManager;
    indexMgr->statManager = statManager;

    if (isNew) {
        Schema *schema = SchemaInit();
        CString *indexnameParam = CStringCreateFromCStr("indexname");
        CString *tablenameParam = CStringCreateFromCStr("tablename");
        CString *fieldnameParam = CStringCreateFromCStr("fieldname");
        
        SchemaAddStringField(schema, indexnameParam, TABLE_MANAGER_MAX_NAME);
        SchemaAddStringField(schema, tablenameParam, TABLE_MANAGER_MAX_NAME);
        SchemaAddStringField(schema, fieldnameParam, TABLE_MANAGER_MAX_NAME);
        
        CString *idxcatTbl = CStringCreateFromCStr("idxcat");
        TableManagerCreateTable(tableManager, idxcatTbl, schema, tx);
        SchemaFree(schema);
        
        CStringDestroy(indexnameParam);
        CStringDestroy(tablenameParam);
        CStringDestroy(fieldnameParam);
        CStringDestroy(idxcatTbl);
    }

    CString *idxcatTbl = CStringCreateFromCStr("idxcat");
    indexMgr->layout = TableManagerGetLayout(tableManager, idxcatTbl, tx);
    CStringDestroy(idxcatTbl);
    return indexMgr;
}

// 创建索引
void IndexMgrCreateIndex(IndexManager *indexMgr, CString *idxname, CString *tblname, CString *fldname, Transaction *tx) {
    CString *idxcatTbl = CStringCreateFromCStr("idxcat");
    CString *indexnameParam = CStringCreateFromCStr("indexname");
    CString *tablenameParam = CStringCreateFromCStr("tablename");
    CString *fieldnameParam = CStringCreateFromCStr("fieldname");
    
    TableScan *ts = TableScanInit(tx, idxcatTbl, indexMgr->layout);
    Scan *scan = ScanInit(ts, SCAN_TABLE_CODE);
    TableScanInsert(scan);
    TableScanSetString(scan, indexnameParam, idxname);
    TableScanSetString(scan, tablenameParam, tblname);
    TableScanSetString(scan, fieldnameParam, fldname);
    TableScanClose(scan);
    
    CStringDestroy(idxcatTbl);
    CStringDestroy(indexnameParam);
    CStringDestroy(tablenameParam);
    CStringDestroy(fieldnameParam);
}

// 获取表的索引信息
map_IndexInfo_t* IndexMgrGetIndexInfo(IndexManager *indexMgr, CString *tblname, Transaction *tx) {
    map_IndexInfo_t *result = malloc(sizeof (map_IndexInfo_t));
    map_init(result);
    
    CString *idxcatTbl = CStringCreateFromCStr("idxcat");
    CString *tablenameParam = CStringCreateFromCStr("tablename");
    CString *indexnameParam = CStringCreateFromCStr("indexname");
    CString *fieldnameParam = CStringCreateFromCStr("fieldname");
    
    TableScan *ts = TableScanInit(tx, idxcatTbl, indexMgr->layout);
    Scan *scan = ScanInit(ts, SCAN_TABLE_CODE);

    while (TableScanNext(scan)) {
        char *currentTblNameStr = TableScanGetString(scan, tablenameParam);
        CString *currentTblName = CStringCreateFromCStr(currentTblNameStr);
        if (CStringEqual(currentTblName, tblname)) {
            char *idxnameStr = TableScanGetString(scan, indexnameParam);
            char *fldnameStr = TableScanGetString(scan, fieldnameParam);

            Layout *tblLayout = TableManagerGetLayout(indexMgr->tableManager, tblname, tx);
            StatInfo *tblStatInfo = StartManagerGetStatInfo(indexMgr->statManager, CStringGetPtr(tblname), tblLayout, tx);
            IndexInfo *indexInfo = IndexInfoInit(idxnameStr, fldnameStr, tblLayout->schema, tx, tblStatInfo);

            map_set(result, fldnameStr, *indexInfo);
        }
        CStringDestroy(currentTblName);
    }

    TableScanClose(scan);
    
    CStringDestroy(idxcatTbl);
    CStringDestroy(tablenameParam);
    CStringDestroy(indexnameParam);
    CStringDestroy(fieldnameParam);
    
    return result;
}

// 释放 IndexMgr
void IndexMgrFree(IndexManager *indexMgr) {
    if (indexMgr) {
        free(indexMgr);
    }
}
