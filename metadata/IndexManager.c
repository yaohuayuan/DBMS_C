//
// Created by yaohuayuan on 2024/11/21.
//

#include "IndexManager.h"
#include <stdlib.h>
#include <string.h>

// 初始化 IndexMgr
IndexManager* IndexMgrInit(bool isNew, TableManager *tableManager, StartManager *statManager, Transaction *tx) {
    IndexManager *indexMgr = (IndexManager*)malloc(sizeof(IndexManager));
    indexMgr->tableManager = tableManager;
    indexMgr->statManager = statManager;

    if (isNew) {
        Schema *schema = SchemaInit();
        SchemaAddStringField(schema, "indexname", TABLE_MANAGER_MAX_NAME);
        SchemaAddStringField(schema, "tablename", TABLE_MANAGER_MAX_NAME);
        SchemaAddStringField(schema, "fieldname", TABLE_MANAGER_MAX_NAME);
        TableManagerCreateTable(tableManager, "idxcat", schema, tx);
        SchemaFree(schema);
    }

    indexMgr->layout = TableManagerGetLayout(tableManager, "idxcat", tx);
    return indexMgr;
}

// 创建索引
void IndexMgrCreateIndex(IndexManager *indexMgr, const char *idxname, const char *tblname, const char *fldname, Transaction *tx) {
    TableScan *ts = TableScanInit(tx, "idxcat", indexMgr->layout);
    TableScanInsert(ts);
    TableScanSetString(ts, "indexname", idxname);
    TableScanSetString(ts, "tablename", tblname);
    TableScanSetString(ts, "fieldname", fldname);
    TableScanClose(ts);
}

// 获取表的索引信息
map_IndexInfo_t* IndexMgrGetIndexInfo(IndexManager *indexMgr, const char *tblname, Transaction *tx) {
    map_IndexInfo_t *result = malloc(sizeof (map_IndexInfo_t));
    map_init(result);
    TableScan *ts = TableScanInit(tx, "idxcat", indexMgr->layout);

    while (TableScanNext(ts)) {
        const char *currentTblName = TableScanGetString(ts, "tablename");
        if (strcmp(currentTblName, tblname) == 0) {
            const char *idxname = TableScanGetString(ts, "indexname");
            const char *fldname = TableScanGetString(ts, "fieldname");

            Layout *tblLayout = TableManagerGetLayout(indexMgr->tableManager, tblname, tx);
            StatInfo *tblStatInfo = StartManagerGetStatInfo(indexMgr->statManager, tblname, tblLayout, tx);
            IndexInfo *indexInfo = IndexInfoInit(idxname, fldname, tblLayout->schema, tx, tblStatInfo);

            map_set(result, fldname, *indexInfo);
        }
    }

    TableScanClose(ts);
    return result;
}

// 释放 IndexMgr
void IndexMgrFree(IndexManager *indexMgr) {
    if (indexMgr) {
        free(indexMgr);
    }
}
