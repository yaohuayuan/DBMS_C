//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_STATINFO_H
#define DBMS_C_STATINFO_H
typedef struct StatInfo {
    int numBlocks;
    int numRecs;
} StatInfo;

// 创建 StatInfo 对象
StatInfo* StatInfoInit(int numBlocks, int numRecs);

// 获取表的块数量
int StatInfoBlocksAccessed(StatInfo* si);

// 获取表的记录数量
int StatInfoRecordsOutput(StatInfo* si);

// 获取指定字段的不同值的数量
int StatInfoDistinctValues(StatInfo* si, const char* fldname);
#endif //DBMS_C_STATINFO_H
