//
// Created by yaohuayuan on 2024/11/11.
//

#include "List.h"
#include "Constant.h"
#include "Term.h"
bool ListCompare(ListData *data1,void *data2,ListDataType type){
    if(type==LIST_TYPE_CONSTANT){
        return ConstantEquals(data1->constantData,(Constant*)data2);
    }else if(type==LIST_TYPE_STRING){
        return strcmp((data1->stringData),(char *)(data2))==0;
    }else if(type == LIST_TYPE_TERM){

    }else if(type == LIST_TYPE_PLAN){

    }
    return false;
}
void ListPrintCMP(ListData *data,ListDataType type){
    if(type==LIST_TYPE_STRING){
        printf("%s",data->stringData);
    }else if(type == LIST_TYPE_CONSTANT){
        Constant *constant =data->constantData;
        if(constant->isInt==true){
            printf("%d",constant->value.ival);
        }else{
            printf("%s",constant->value.sval);
        }

    }else if(type == LIST_TYPE_TERM){

        printf("%s",(TermToString(data->termData)));
    }
}
// 初始化链表
List* ListInit(ListDataType type) {
    List *list = (List *)malloc(sizeof(List));
    list->head = NULL;
    list->size = 0;
    list->type = type;
    return list;
}

// 向链表末尾添加节点
void ListAppend(List *list, void *data, int length) {
    if (!list) return;

    // 创建新节点
    ListNode *newNode = (ListNode *)malloc(sizeof(ListNode));
    newNode->data = malloc(sizeof(ListData));
    ListDataType type = list->type;
    if(type==LIST_TYPE_STRING){
        newNode->data->stringData = strdup((char*)data);
    }else if(type==LIST_TYPE_CONSTANT){
        newNode->data->constantData = (Constant*)data;
    }else if(type==LIST_TYPE_PLAN){
        newNode->data->planData = (Plan*)data;
    }else if(type==LIST_TYPE_TERM){
        newNode->data->termData = (Term*)data;
    }

    newNode->next = NULL;

    // 如果链表为空，直接设置为头节点
    if (!list->head) {
        list->head = newNode;
    } else {
        // 否则找到链表末尾并追加
        ListNode *current = list->head;
        while (current->next) {
            current = current->next;
        }
        current->next = newNode;
    }
    list->size++;
}

// 检查链表中是否包含某值
bool ListContains(List *list, void *data) {
    if (!list || !list->head) return false;

    ListNode *current = list->head;
    while (current) {
        if (ListCompare(current->data, data,list->type)) {
            return true;  // 找到匹配值
        }
        current = current->next;
    }
    return false;  // 未找到匹配值
}

// 删除链表中与给定值匹配的节点
void ListRemoveByValue(List *list, void *value) {
    if (!list || !list->head) return;

    ListNode *current = list->head;
    ListNode *prev = NULL;

    while (current) {
        if (ListCompare(current->data, value,list->type)) {
            if (!prev) {
                // 删除头节点
                list->head = current->next;
            } else {
                // 删除中间或尾部节点
                prev->next = current->next;
            }

            // 释放节点内存
            free(current->data);
            free(current);
            list->size--;
            return;  // 删除第一个匹配的节点后返回
        }
        prev = current;
        current = current->next;
    }
}

// 打印链表
void ListPrint(List *list) {
    if (!list || !list->head) {
        printf("List is empty.\n");
        return;
    }

    ListNode *current = list->head;
    while (current) {
        ListPrintCMP(current->data,list->type);
        printf(" -> ");
        current = current->next;
    }
    printf("NULL\n");
}

// 释放链表
void ListFree(List *list) {
    if (!list) return;

    ListNode *current = list->head;
    while (current) {
        ListNode *next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(list);
}
void ListAddAll(List*list1,List*list2){
    ListNode *current = list2->head;
    while (current) {
        if(list1->type == LIST_TYPE_STRING){
            ListAppend(list1,current->data->stringData, 0);
        }else if(list1->type == LIST_TYPE_CONSTANT){
            ListAppend(list1,current->data->constantData, 0);
        }else if(list1->type == LIST_TYPE_TERM){
            ListAppend(list1,current->data->termData,0);
        }else if(list1->type == LIST_TYPE_PLAN){
            ListAppend(list1,current->data->planData,0);
        }
        current = current->next;
    }

}
// 根据索引删除链表中的节点并返回其数据值
void *ListRemoveByIndex(List *list, int index) {
    if (!list || !list->head || index < 0 || index >= list->size) {
        printf("Invalid index or empty list.\n");
        return NULL;
    }

    ListNode *current = list->head;
    ListNode *prev = NULL;

    // 遍历到指定索引
    for (int i = 0; i < index; i++) {
        prev = current;
        current = current->next;
    }

    // 删除节点
    if (!prev) {
        // 删除头节点
        list->head = current->next;
    } else {
        // 删除中间或尾部节点
        prev->next = current->next;
    }
    void *data = NULL;
    if(list->type == LIST_TYPE_TERM){
        data = current->data->termData;
    }else if(list->type == LIST_TYPE_CONSTANT){
        data = current->data->constantData;
    }else if(list->type == LIST_TYPE_STRING){
        data = current->data->stringData;
    }else if(list->type == LIST_TYPE_PLAN){
        data = current->data->planData;
    }

    // 释放节点结构
    free(current);
    list->size--;

    return data;  // 返回数据
}
