//
// Created by yaohuayuan on 2024/11/21.
//

#include "IndexInfo.h"
#include <stdlib.h>
#include <string.h>

// 初始化 IndexInfo 对象
IndexInfo* IndexInfoInit(const char* idxname, const char* fldname, Schema* tblSchema, Transaction* transaction, StatInfo* statInfo) {
    IndexInfo* indexInfo = (IndexInfo*)malloc(sizeof(IndexInfo));
    indexInfo->idxname = CStringCreateFromCStr(idxname);
    indexInfo->fldname = CStringCreateFromCStr(fldname);
    indexInfo->tblSchema = tblSchema;
    indexInfo->transaction = transaction;
    indexInfo->statInfo = statInfo;
    indexInfo->layout = IndexInfoCreateIdxLayout(indexInfo);
    return indexInfo;
}

// 释放 IndexInfo 对象
void IndexInfoFree(IndexInfo* indexInfo) {
    if (!indexInfo) return;
    CStringDestroy(indexInfo->idxname);
    CStringDestroy(indexInfo->fldname);
    // Layout 不需要释放，因为是其他模块管理
    free(indexInfo);
}

// 创建索引布局
Layout* IndexInfoCreateIdxLayout(IndexInfo* indexInfo) {
    Schema* schema = SchemaInit();
    CString *blockParam = CStringCreateFromCStr("block");
    CString *idParam = CStringCreateFromCStr("id");
    CString *datavalParam = CStringCreateFromCStr("dataval");
    CString *fldnameParam = CStringCreateFromCStr(CStringGetPtr(indexInfo->fldname));
    
    SchemaAddIntField(schema, blockParam);
    SchemaAddIntField(schema, idParam);

    if (SchemaType(indexInfo->tblSchema, fldnameParam) == FILE_INFO_CODE_INTEGER) {
        SchemaAddIntField(schema, datavalParam);
    } else {
        int fldlen = SchemaLength(indexInfo->tblSchema, fldnameParam);
        SchemaAddStringField(schema, datavalParam, fldlen);
    }
    
    CStringDestroy(blockParam);
    CStringDestroy(idParam);
    CStringDestroy(datavalParam);
    CStringDestroy(fldnameParam);
    
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
    return StatInfoRecordsOutput(indexInfo->statInfo) / StatInfoDistinctValues(indexInfo->statInfo, CStringGetPtr(indexInfo->fldname));
}

// 返回指定字段的不同值数量
int IndexInfoDistinctValues(IndexInfo* indexInfo, const char* fname) {
    if (strcmp(CStringGetPtr(indexInfo->fldname), fname) == 0) {
        return 1;
    }
    return StatInfoDistinctValues(indexInfo->statInfo, fname);
}
