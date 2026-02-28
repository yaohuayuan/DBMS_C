//
// Created by yaohuayuan on 2024/11/30.
//

#ifndef DBMS_C_QUERYDATA_H
#define DBMS_C_QUERYDATA_H
#include "List.h"
#include "Predicate.h"
/**
 * @brief QueryData 结构体定义，用于存储查询操作的数据。
 */
typedef struct QueryData{
    List *fields;    ///< 查询的字段列表
    List* tables;    ///< 查询的表列表
    Predicate *predicate; ///< 查询条件
}QueryData;

/**
 * @brief 初始化一个新的 QueryData 实例。
 *
 * @param fields 查询的字段列表。
 * @param tables 查询的表列表。
 * @param predicate 查询条件。
 * @return 返回初始化后的 QueryData 指针。
 */
QueryData *QueryDataInit(List*fields,List*tables,Predicate *predicate);

/**
 * @brief 将 QueryData 转换为字符串表示。
 *
 * @param queryData 指向 QueryData 实例的指针。
 * @return 返回 QueryData 的字符串表示。
 */
char *QueryDataToString(QueryData*queryData);
#endif //DBMS_C_QUERYDATA_H
