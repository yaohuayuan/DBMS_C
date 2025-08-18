//
// Created by yaohuayuan on 2024/11/21.
//

#ifndef DBMS_C_HASHINDEX_H
#define DBMS_C_HASHINDEX_H

#include "Transaction.h"
#include "Layout.h"
#include "Constant.h"
#include "TableScan.h"

/**
 * @brief 定义哈希索引桶的数量。
 */
#define HASH_INDEX_NUM_BUCKETS 100

/**
 * @brief HashIndex 结构体定义，用于实现基于哈希的索引。
 */
typedef struct HashIndex {
    Transaction *transaction; ///< 指向当前事务的指针
    char *idxname;            ///< 索引名称
    Layout *layout;           ///< 表布局信息
    Constant *constant;       ///< 当前处理的常量值
    Scan *scan;               ///< 扫描器，用于遍历表数据
} HashIndex;

/**
 * @brief 初始化一个新的 HashIndex 实例。
 *
 * @param transaction 指向当前事务的指针。
 * @param idxname 索引名称。
 * @param layout 表布局信息。
 * @return 返回初始化后的 HashIndex 指针。
 */
HashIndex* HashIndexInit(Transaction* transaction, char *idxname, Layout* layout);

/**
 * @brief 关闭并释放 HashIndex 实例及其资源。
 *
 * @param hashIndex 指向 HashIndex 的指针。
 */
void HashIndexClose(HashIndex* hashIndex);

/**
 * @brief 设置扫描位置到第一个匹配项之前。
 *
 * @param hashIndex 指向 HashIndex 的指针。
 * @param constant 搜索条件的常量值。
 */
void HashBeforeFirst(HashIndex* hashIndex, Constant *constant);

/**
 * @brief 移动到下一个匹配项。
 *
 * @param hashIndex 指向 HashIndex 的指针。
 * @return 如果存在下一个匹配项，则返回 true；否则返回 false。
 */
bool HashIndexNext(HashIndex* hashIndex);

/**
 * @brief 获取当前匹配项的数据记录ID (RID)。
 *
 * @param hashIndex 指向 HashIndex 的指针。
 * @return 返回当前匹配项的 RID 指针。
 */
RID* HashIndexGetDataRID(HashIndex* hashIndex);

/**
 * @brief 向哈希索引中插入一个新的键值对。
 *
 * @param hashIndex 指向 HashIndex 的指针。
 * @param val 要插入的键值。
 * @param rid 对应的数据记录ID (RID)。
 */
void HashIndexInsert(HashIndex* hashIndex, Constant* val, RID* rid);

/**
 * @brief 从哈希索引中删除一个键值对。
 *
 * @param hashIndex 指向 HashIndex 的指针。
 * @param val 要删除的键值。
 * @param rid 对应的数据记录ID (RID)。
 */
void HashIndexDelete(HashIndex *hashIndex, Constant *val, RID* rid);

/**
 * @brief 计算哈希索引的搜索成本。
 *
 * @param numblocks 数据块数量。
 * @param rpb 每个块中的记录数。
 * @return 返回估计的搜索成本。
 */
int HashIndexSearchCost(int numblocks, int rpb);

#endif //DBMS_C_HASHINDEX_H