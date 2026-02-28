#include "List.h" 
#include <stdlib.h> 
#include <stdio.h> 
#include "CString.h"
#include "Constant.h"
#include"Term.h"
/* 函数声明，这些函数将在ListInit中根据type自动设置 */

// LIST_TYPE_STRING 对应的函数
void string_free(void *data) {
    CStringDestroy((CString *)data);
}

bool string_equals(void *a, void *b) {
    return CStringEqual((CString *)a, (CString *)b);
}

void string_print(void *data) {
    printf("%s", CStringGetPtr((CString *)data));
}

// LIST_TYPE_CONSTANT 对应的函数
void constant_free(void *data) {

    ConstantFree((Constant *)data);
}

bool constant_equals(void *a, void *b) {
    // 假设Constant有equals函数
    return ConstantEquals((Constant *)a, (Constant *)b);
}

void constant_print(void *data) {
    // 假设Constant有toString函数

    char *str = ConstantToString((Constant *)data);
    printf("%s", str);
    free(str);
}

// LIST_TYPE_TERM 对应的函数
void term_free(void *data) {
    // Term的释放逻辑

    TermFree((Term *)data);
}

bool term_equals(void *a, void *b) {
    // Term的比较逻辑

    return TermEquals((Term *)a, (Term *)b);
}

void term_print(void *data) {
    // Term的打印逻辑

    char *str = TermToString((Term *)data);
    printf("%s", str);
    free(str);
}

// LIST_TYPE_PLAN 对应的函数
void plan_free(void *data) {
    // Plan的释放逻辑，如果需要的话
    // extern void PlanFree(Plan *plan);
    // PlanFree((Plan *)data);
}

bool plan_equals(void *a, void *b) {
    // Plan的比较逻辑，如果需要的话
    return false;
}

void plan_print(void *data) {
    // Plan的打印逻辑，如果需要的话
    printf("Plan");
}

List *ListInit( 
    ListDataType type, 
    ListFreeFunc freeFunc, 
    ListEqualsFunc equalsFunc, 
    ListPrintFunc printFunc 
) {
    List *list = (List *)malloc(sizeof(List)); 
    if (!list) return NULL; 

    list->head = NULL; 
    list->size = 0; 
    list->type = type; 
    
    // 根据type自动设置函数指针，如果提供了自定义函数，则优先使用自定义函数
    switch (type) {
        case LIST_TYPE_STRING:
            list->freeFunc = freeFunc ? freeFunc : string_free;
            list->equalsFunc = equalsFunc ? equalsFunc : string_equals;
            list->printFunc = printFunc ? printFunc : string_print;
            break;
        case LIST_TYPE_CONSTANT:
            list->freeFunc = freeFunc ? freeFunc : constant_free;
            list->equalsFunc = equalsFunc ? equalsFunc : constant_equals;
            list->printFunc = printFunc ? printFunc : constant_print;
            break;
        case LIST_TYPE_TERM:
            list->freeFunc = freeFunc ? freeFunc : term_free;
            list->equalsFunc = equalsFunc ? equalsFunc : term_equals;
            list->printFunc = printFunc ? printFunc : term_print;
            break;
        case LIST_TYPE_PLAN:
            list->freeFunc = freeFunc ? freeFunc : plan_free;
            list->equalsFunc = equalsFunc ? equalsFunc : plan_equals;
            list->printFunc = printFunc ? printFunc : plan_print;
            break;
        default:
            list->freeFunc = freeFunc;
            list->equalsFunc = equalsFunc;
            list->printFunc = printFunc;
            break;
    }

    return list; 
} 

void ListAppend(List *list, void *data) { 
    if (!list) return; 

    ListNode *node = (ListNode *)malloc(sizeof(ListNode)); 
    if (!node) return; 

    node->value.raw = data; 
    node->next = NULL; 

    if (!list->head) { 
        list->head = node; 
    } else { 
        ListNode *cur = list->head; 
        while (cur->next) cur = cur->next; 
        cur->next = node; 
    } 

    list->size++; 
} 

bool ListContains(List *list, void *data) { 
    if (!list || !list->equalsFunc) return false; 

    for (ListNode *cur = list->head; cur; cur = cur->next) { 
        if (list->equalsFunc(cur->value.raw, data)) { 
            return true; 
        } 
    } 
    return false; 
} 

void ListRemoveByValue(List *list, void *data) { 
    if (!list || !list->equalsFunc) return; 

    ListNode *cur = list->head; 
    ListNode *prev = NULL; 

    while (cur) { 
        if (list->equalsFunc(cur->value.raw, data)) { 
            if (prev) { 
                prev->next = cur->next; 
            } else { 
                list->head = cur->next; 
            } 

            if (list->freeFunc) { 
                list->freeFunc(cur->value.raw); 
            } 
            free(cur); 
            list->size--; 
            return; 
        } 
        prev = cur; 
        cur = cur->next; 
    } 
} 

void *ListRemoveByIndex(List *list, int index) { 
    if (!list || index < 0 || index >= list->size) return NULL; 

    ListNode *cur = list->head; 
    ListNode *prev = NULL; 

    for (int i = 0; i < index; i++) { 
        prev = cur; 
        cur = cur->next; 
    } 

    if (prev) { 
        prev->next = cur->next; 
    } else { 
        list->head = cur->next; 
    } 

    void *data = cur->value.raw; 
    free(cur); 
    list->size--; 

    return data; 
} 

// 辅助函数：根据类型创建数据的深拷贝
void *copyDataByType(void *data, ListDataType type) {
    switch (type) {
        case LIST_TYPE_STRING:
            return CStringCreateFromCString((CString *)data);
        case LIST_TYPE_CONSTANT: {
            Constant *c = (Constant *)data;
            if (c->isInt) {
                return ConstantInitInt(c->value.ival);
            } else {
                return ConstantInitCString(c->value.sval);
            }
        }
        case LIST_TYPE_TERM:
            // 假设Term有拷贝函数
            // 暂时返回原指针，需要后续实现TermCopy
            return data;
        case LIST_TYPE_PLAN:
            // 假设Plan有拷贝函数
            // 暂时返回原指针，需要后续实现PlanCopy
            return data;
        default:
            return data;
    }
}

void ListAddAll(List *dest, List *src) {
    if (!dest || !src) return;

    for (ListNode *cur = src->head; cur; cur = cur->next) {
        // 根据类型创建深拷贝
        void *copy = copyDataByType(cur->value.raw, src->type);
        ListAppend(dest, copy);
    }
} 

void ListPrint(List *list) { 
    if (!list || !list->printFunc) return; 

    printf("["); 
    ListNode *cur = list->head; 
    while (cur) { 
        list->printFunc(cur->value.raw); 
        if (cur->next) printf(", "); 
        cur = cur->next; 
    } 
    printf("]\n"); 
} 

void ListFree(List *list) { 
    if (!list) return; 

    ListNode *cur = list->head; 
    while (cur) { 
        ListNode *next = cur->next; 
        if (list->freeFunc) { 
            list->freeFunc(cur->value.raw); 
        } 
        free(cur); 
        cur = next; 
    } 

    free(list); 
}