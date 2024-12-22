//
// Created by yaohuayuan on 2024/11/11.
//

#ifndef DBMS_C_LIST_H
#define DBMS_C_LIST_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Constant.h"

typedef struct Constant Constant;
typedef struct Plan Plan;
typedef struct Term Term;

/**
 * @brief ListData 结构体定义，用于封装链表节点的数据。
 */
typedef struct ListData {
    char *stringData;          ///< 字符串数据指针
    Constant *constantData;    ///< 常量数据指针
    Plan *planData;            ///< 计划数据指针
    Term *termData;            ///< 条件数据指针
} ListData;

/**
 * @brief 链表节点结构体定义。
 */
typedef struct ListNode {
    ListData *data;            ///< 数据指针，存储任意类型的数据
    int length;                ///< 数据长度（适用于某些数据类型）
    struct ListNode *next;     ///< 指向下一个节点的指针
} ListNode;

/**
 * @brief 链表数据类型的枚举定义。
 */
typedef enum ListDataType {
    LIST_TYPE_STRING,          ///< 字符串类型
    LIST_TYPE_CONSTANT,        ///< 常量类型
    LIST_TYPE_TERM,            ///< 条件类型
    LIST_TYPE_PLAN             ///< 计划类型
} ListDataType;

/**
 * @brief 链表结构体定义。
 */
typedef struct List {
    ListNode *head;            ///< 链表头节点指针
    int size;                  ///< 链表中的节点数
    ListDataType type;         ///< 链表中所有节点的数据类型
} List;

/**
 * @brief 初始化一个新的链表实例。
 *
 * @param type 链表中所有节点的数据类型。
 * @return 返回初始化后的链表指针。
 */
List* ListInit(ListDataType type);

/**
 * @brief 添加一个新节点到链表尾部。
 *
 * @param list 指向链表的指针。
 * @param data 要添加的数据指针。
 * @param length 数据长度（适用于某些数据类型）。
 */
void ListAppend(List *list, void *data, int length);

/**
 * @brief 检查链表中是否包含特定值。
 *
 * @param list 指向链表的指针。
 * @param data 要查找的数据指针。
 * @return 如果找到匹配项，则返回 true；否则返回 false。
 */
bool ListContains(List *list, void *data);

/**
 * @brief 打印链表的内容。
 *
 * @param list 指向链表的指针。
 */
void ListPrint(List *list);

/**
 * @brief 释放链表及其所有节点占用的内存。
 *
 * @param list 指向链表的指针。
 */
void ListFree(List *list);

/**
 * @brief 删除链表中第一个与给定值匹配的节点。
 *
 * @param list 指向链表的指针。
 * @param value 要删除的值。
 */
void ListRemoveByValue(List *list, void *value);

/**
 * @brief 将一个链表的所有元素添加到另一个链表中。
 *
 * @param list1 目标链表。
 * @param list2 源链表。
 */
void ListAddAll(List *list1, List *list2);

/**
 * @brief 根据索引删除并返回链表中的节点。
 *
 * @param list 指向链表的指针。
 * @param index 要删除的节点索引。
 * @return 返回被删除节点的数据指针；如果索引无效或链表为空，则返回 NULL。
 */
void *ListRemoveByIndex(List *list, int index);

#endif // DBMS_C_LIST_H