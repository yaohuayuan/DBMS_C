//
// Created by yaohuayuan on 2024/11/21.
//

#include "MetadataManager.h"
#include <stdlib.h>
#include <string.h>

// 初始化 MetadataMgr
MetadataMgr* MetadataMgrInit(bool isNew, Transaction *tx) {
    MetadataMgr *mdMgr = (MetadataMgr*)malloc(sizeof(MetadataMgr));
    mdMgr->tblMgr = TableManagerInit(isNew, tx);
    mdMgr->viewMgr = ViewManagerInit(isNew, mdMgr->tblMgr, tx);
    mdMgr->statMgr = StartManagerInit(mdMgr->tblMgr, tx);
    mdMgr->idxMgr = IndexMgrInit(isNew, mdMgr->tblMgr, mdMgr->statMgr, tx);
    return mdMgr;
}

// 创建表
void MetadataMgrCreateTable(MetadataMgr *mdMgr, const char *tblname, Schema *schema, Transaction *tx) {
    TableManagerCreateTable(mdMgr->tblMgr, tblname, schema, tx);
}

// 获取表布局
Layout* MetadataMgrGetLayout(MetadataMgr *mdMgr, const char *tblname, Transaction *tx) {
    return TableManagerGetLayout(mdMgr->tblMgr, tblname, tx);
}

// 创建视图
void MetadataMgrCreateView(MetadataMgr *mdMgr, const char *viewname, const char *viewdef, Transaction *tx) {
    ViewManagerCreateView(mdMgr->viewMgr, viewname, viewdef, tx);
}

// 获取视图定义
char* MetadataMgrGetViewDef(MetadataMgr *mdMgr, const char *viewname, Transaction *tx) {
    return ViewManagerGetViewDef(mdMgr->viewMgr, viewname, tx);
}

// 创建索引
void MetadataMgrCreateIndex(MetadataMgr *mdMgr, const char *idxname, const char *tblname, const char *fldname, Transaction *tx) {
    IndexMgrCreateIndex(mdMgr->idxMgr, idxname, tblname, fldname, tx);
}

// 获取索引信息
map_IndexInfo_t* MetadataManagerGetIndexInfo(MetadataMgr *mdMgr, const char *tblname, Transaction *tx) {
    return IndexMgrGetIndexInfo(mdMgr->idxMgr, tblname, tx);
}

// 获取统计信息
StatInfo* MetadataMgrGetStatInfo(MetadataMgr *mdMgr, const char *tblname, Layout *layout, Transaction *tx) {
    return StartManagerGetStatInfo(mdMgr->statMgr, tblname, layout, tx);
}

// 释放 MetadataMgr

