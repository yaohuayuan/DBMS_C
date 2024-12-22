// Created by yaohuayuan on 2024/11/19.

#ifndef DBMS_C_STATINFO_H
#define DBMS_C_STATINFO_H

/**
 * @brief StatInfo 结构体，用于存储表的统计信息，包括块数和记录数。
 */
typedef struct StatInfo {
    int numBlocks; ///< 表的块数量
    int numRecs;   ///< 表的记录数量
} StatInfo;

/**
 * @brief 创建并初始化一个新的 StatInfo 对象。
 *
 * @param numBlocks 表的块数量。
 * @param numRecs 表的记录数量。
 * @return 返回初始化后的 StatInfo 指针。
 */
StatInfo* StatInfoInit(int numBlocks, int numRecs);

/**
 * @brief 获取表的块数量。
 *
 * @param si 指向 StatInfo 的指针。
 * @return 返回表的块数量。
 */
int StatInfoBlocksAccessed(StatInfo* si);

/**
 * @brief 获取表的记录数量。
 *
 * @param si 指向 StatInfo 的指针。
 * @return 返回表的记录数量。
 */
int StatInfoRecordsOutput(StatInfo* si);

/**
 * @brief 获取指定字段的不同值的数量。
 *
 * 这个函数的实现依赖于具体的数据库设计，可能需要额外的字段信息来计算。
 *
 * @param si 指向 StatInfo 的指针。
 * @param fldname 字段名称。
 * @return 返回指定字段的不同值的数量。
 */
int StatInfoDistinctValues(StatInfo* si, const char* fldname);

#endif // DBMS_C_STATINFO_H
