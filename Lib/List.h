// 
// Created by yaohuayuan on 2026/01/20 
// 

#ifndef DBMS_C_LIST_H 
#define DBMS_C_LIST_H 

#include <stdbool.h> 
#include <stddef.h> 
#include "CString.h"

/* 前置声明，避免头文件互相依赖 */ 
typedef struct Constant Constant; 
typedef struct Term Term; 
typedef struct Plan Plan; 

/** 
 * @brief 链表中元素的数据类型 
 */ 
typedef enum { 
    LIST_TYPE_STRING, 
    LIST_TYPE_CONSTANT, 
    LIST_TYPE_TERM, 
    LIST_TYPE_PLAN 
} ListDataType; 

/** 
 * @brief 链表中存储的数据（联合体） 
 */ 
typedef union { 
    CString *stringData; 
    Constant *constantData; 
    Term *termData; 
    Plan *planData; 
    void *raw;          ///< 泛化访问 
} ListValue; 

/* 函数指针：定义 List 的“行为” */ 
typedef void (*ListFreeFunc)(void *data); 
typedef bool (*ListEqualsFunc)(void *a, void *b); 
typedef void (*ListPrintFunc)(void *data); 

/** 
 * @brief 链表节点 
 */ 
typedef struct ListNode { 
    ListValue value; 
    struct ListNode *next; 
} ListNode; 

/** 
 * @brief 链表结构 
 */ 
typedef struct List { 
    ListNode *head; 
    int size; 
    ListDataType type; 

    ListFreeFunc freeFunc; 
    ListEqualsFunc equalsFunc; 
    ListPrintFunc printFunc; 
} List; 

/* ================= 接口定义 ================= */ 

List *ListInit( 
    ListDataType type, 
    ListFreeFunc freeFunc, 
    ListEqualsFunc equalsFunc, 
    ListPrintFunc printFunc 
); 

void ListAppend(List *list, void *data); 

bool ListContains(List *list, void *data); 

void ListRemoveByValue(List *list, void *data); 

void *ListRemoveByIndex(List *list, int index); 

void ListAddAll(List *dest, List *src); 

void ListPrint(List *list); 

void ListFree(List *list); 

#endif // DBMS_C_LIST_H