// Created by yaohuayuan on 2024/11/21.

#ifndef DBMS_C_INDEXINFO_H
#define DBMS_C_INDEXINFO_H

#include "Transaction.h"
#include "Schema.h"
#include "Layout.h"
#include "StatInfo.h"
#include "HashIndex.h"
#include "CString.h"

/**
 * @brief IndexInfo 结构体定义，用于存储索引信息。
 */
typedef struct IndexInfo {
    CString *idxname;            ///< 索引名称
    CString *fldname;            ///< 字段名称，该字段上创建了索引
    Transaction *transaction; ///< 指向事务的指针
    Schema *tblSchema;        ///< 表的模式信息
    Layout *layout;           ///< 表的布局信息
    StatInfo *statInfo;       ///< 表的统计信息
} IndexInfo;

/**
 * @brief 初始化一个新的 IndexInfo 实例。
 *
 * @param idxname 索引名称。
 * @param fldname 字段名称。
 * @param tblSchema 表模式信息。
 * @param transaction 当前事务。
 * @param statInfo 表的统计信息。
 * @return 返回初始化后的 IndexInfo 指针。
 */
IndexInfo* IndexInfoInit(const char* idxname, const char* fldname, Schema* tblSchema, Transaction* transaction, StatInfo* statInfo);

/**
 * @brief 释放 IndexInfo 实例占用的内存。
 *
 * @param indexInfo 指向 IndexInfo 的指针。
 */
void IndexInfoFree(IndexInfo* indexInfo);

/**
 * @brief 为索引创建一个布局。
 *
 * @param indexInfo 指向 IndexInfo 的指针。
 * @return 返回为索引创建的布局。
 */
Layout* IndexInfoCreateIdxLayout(IndexInfo* indexInfo);

/**
 * @brief 获取访问索引所需的块数。
 *
 * @param indexInfo 指向 IndexInfo 的指针。
 * @return 返回块的数量。
 */
int IndexInfoBlocksAccessed(IndexInfo* indexInfo);

/**
 * @brief 获取输出记录的数量。
 *
 * @param indexInfo 指向 IndexInfo 的指针。
 * @return 返回输出记录的数量。
 */
int IndexInfoRecordsOutput(IndexInfo* indexInfo);

/**
 * @brief 获取指定字段的不同值数量。
 *
 * @param indexInfo 指向 IndexInfo 的指针。
 * @param fname 字段名称。
 * @return 返回字段的不同值数量。
 */
int IndexInfoDistinctValues(IndexInfo* indexInfo, const char* fname);

#endif // DBMS_C_INDEXINFO_H
