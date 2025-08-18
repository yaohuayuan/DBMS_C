//
// Created by yaohuayuan on 2024/11/30.
//

#include "QueryData.h"
QueryData *QueryDataInit(List*fields,List*tables,Predicate *predicate){
    QueryData *queryData = malloc(sizeof (QueryData));
    queryData->predicate = predicate;
    queryData->fields = fields;
    queryData->tables = tables;
    return queryData;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char* QueryDataToString(QueryData *qd) {
    if (!qd) return strdup("");  // 如果 QueryData 为空，直接返回空字符串

    // 预估最大缓冲区大小（根据字段和表名的个数来估算，假设每个字段/表名平均长度为 100）
    size_t buffer_size = 1024;
    char *result = (char *)malloc(buffer_size);
    if (!result) return NULL;  // 内存分配失败处理

    // 初始化结果字符串
    strcpy(result, "select ");

    // 拼接字段名
    ListNode *fieldNode = qd->fields->head;
    size_t offset = strlen(result);
    while (fieldNode) {
        char *fieldName = fieldNode->data->stringData;
        size_t len = strlen(fieldName);

        // 确保缓冲区足够大
        if (offset + len + 2 > buffer_size) {
            buffer_size *= 2;  // 扩展缓冲区
            result = (char *)realloc(result, buffer_size);
            if (!result) return NULL;  // 内存分配失败处理
        }

        // 拼接字段名
        strcat(result, fieldName);
        strcat(result, ", ");
        offset = strlen(result);  // 更新偏移量
        fieldNode = fieldNode->next;
    }
    if (qd->fields->size > 0) {
        result[strlen(result) - 2] = '\0';  // 移除最后的逗号和空格
    }

    // 拼接表名
    strcat(result, " from ");
    offset = strlen(result);

    ListNode *tableNode = qd->tables->head;
    while (tableNode) {
        char *tableName = tableNode->data->stringData;
        size_t len = strlen(tableName);

        // 确保缓冲区足够大
        if (offset + len + 2 > buffer_size) {
            buffer_size *= 2;  // 扩展缓冲区
            result = (char *)realloc(result, buffer_size);
            if (!result) return NULL;  // 内存分配失败处理
        }

        // 拼接表名
        strcat(result, tableName);
        strcat(result, ", ");
        offset = strlen(result);  // 更新偏移量
        tableNode = tableNode->next;
    }
    if (qd->tables->size > 0) {
        result[strlen(result) - 2] = '\0';  // 移除最后的逗号和空格
    }

    // 拼接谓词
    if (qd->predicate) {
        char *predString = PredicateToString(qd->predicate);  // 假设已有 PredicateToString 实现
        if (strlen(predString) > 0) {
            // 确保缓冲区足够大
            size_t len = strlen(predString);
            if (offset + strlen(" where ") + len + 1 > buffer_size) {
                buffer_size *= 2;  // 扩展缓冲区
                result = (char *)realloc(result, buffer_size);
                if (!result) return NULL;  // 内存分配失败处理
            }

            // 拼接 "where" 和谓词
            strcat(result, " where ");
            strcat(result, predString);
        }
        free(predString);
    }

    // 添加结尾空格或分号
    size_t result_len = strlen(result);
    if (result[result_len - 1] != ' ' && result[result_len - 1] != ';') {
        // 如果最后一个字符既不是空格也不是分号，则添加分号
        strcat(result, " ;");
    }

    return result;
}
