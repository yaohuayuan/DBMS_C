//
// Created by yaohuayuan on 2024/11/11.
//

#ifndef DBMS_C_LIST_H
#define DBMS_C_LIST_H

#include <stddef.h>

// 定义链表节点结构体
typedef struct Node {
    void *data;           // 数据指针，存储任意类型的数据
    struct Node *next;    // 指向下一个节点
} Node;
typedef struct LinkedList {
    Node *head;           // 指向链表头节点
    size_t data_size;     // 数据类型的大小
} LinkedList;
LinkedList* LinkedListInit(size_t data_size);
void appendNode(LinkedList *list, void *data);
void printList(LinkedList *list, void (*printFunc)(void *));
void freeList(LinkedList *list);

#endif //DBMS_C_LIST_H
