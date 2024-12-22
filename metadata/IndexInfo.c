//
// Created by yaohuayuan on 2024/11/21.
//

#include "IndexInfo.h"
#include <stdlib.h>
#include <string.h>

// 初始化 IndexInfo 对象
IndexInfo* IndexInfoInit(const char* idxname, const char* fldname, Schema* tblSchema, Transaction* transaction, StatInfo* statInfo) {
    IndexInfo* indexInfo = (IndexInfo*)malloc(sizeof(IndexInfo));
    indexInfo->idxname = strdup(idxname);
    indexInfo->fldname = strdup(fldname);
    indexInfo->tblSchema = tblSchema;
    indexInfo->transaction = transaction;
    indexInfo->statInfo = statInfo;
    indexInfo->layout = IndexInfoCreateIdxLayout(indexInfo);
    return indexInfo;
}

// 释放 IndexInfo 对象
void IndexInfoFree(IndexInfo* indexInfo) {
    if (!indexInfo) return;
    free(indexInfo->idxname);
    free(indexInfo->fldname);
    // Layout 不需要释放，因为是其他模块管理
    free(indexInfo);
}

// 创建索引布局
Layout* IndexInfoCreateIdxLayout(IndexInfo* indexInfo) {
    Schema* schema = SchemaInit();
    SchemaAddIntField(schema, "block");
    SchemaAddIntField(schema, "id");

    if (SchemaType(indexInfo->tblSchema, indexInfo->fldname) == FILE_INFO_CODE_INTEGER) {
        SchemaAddIntField(schema, "dataval");
    } else {
        int fldlen = SchemaLength(indexInfo->tblSchema, indexInfo->fldname);
        SchemaAddStringField(schema, "dataval", fldlen);
    }
    return LayoutInit(schema,NULL,-1);
}

// 估计索引访问的块数
int IndexInfoBlocksAccessed(IndexInfo* indexInfo) {
    int rpb = TransactionBlockSize(indexInfo->transaction) / (indexInfo->layout->SlotSize);
    int numblocks = StatInfoRecordsOutput(indexInfo->statInfo) / rpb;
    return HashIndexSearchCost(numblocks, rpb);
}

// 返回具有特定搜索键的记录数
int IndexInfoRecordsOutput(IndexInfo* indexInfo) {
    return StatInfoRecordsOutput(indexInfo->statInfo) / StatInfoDistinctValues(indexInfo->statInfo, indexInfo->fldname);
}

// 返回指定字段的不同值数量
int IndexInfoDistinctValues(IndexInfo* indexInfo, const char* fname) {
    if (strcmp(indexInfo->fldname, fname) == 0) {
        return 1;
    }
    return StatInfoDistinctValues(indexInfo->statInfo, fname);
}
