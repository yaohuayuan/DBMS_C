#ifndef DBMS_C_CLIST_H
#define DBMS_C_CLIST_H

#include <stdbool.h>
#include <stddef.h>

typedef void (*CListFreeFunc)(void *data);
typedef bool (*CListEqualsFunc)(const void *a, const void *b);
typedef void (*CListPrintFunc)(const void *data);
typedef void *(*CListCopyFunc)(const void *data);
typedef bool (*CListVisitFunc)(void *data, void *context);

typedef struct CListNode {
    void *data;
    struct CListNode *next;
} CListNode;

typedef struct CList {
    CListNode *head;
    CListNode *tail;
    size_t size;
    CListFreeFunc freeFunc;
    CListEqualsFunc equalsFunc;
    CListPrintFunc printFunc;
    CListCopyFunc copyFunc;
} CList;

CList *CListInit(
    CListFreeFunc freeFunc,
    CListEqualsFunc equalsFunc,
    CListPrintFunc printFunc
);

CList *CListInitWithCopy(
    CListFreeFunc freeFunc,
    CListEqualsFunc equalsFunc,
    CListPrintFunc printFunc,
    CListCopyFunc copyFunc
);

void CListSetCopyFunc(CList *list, CListCopyFunc copyFunc);
bool CListAppend(CList *list, void *data);
bool CListAppendCopy(CList *list, const void *data);
void *CListGet(const CList *list, size_t index);
bool CListIsEmpty(const CList *list);
long CListIndexOf(const CList *list, const void *data);
bool CListContains(const CList *list, const void *data);
bool CListRemoveByValue(CList *list, const void *data);
void *CListRemoveByIndex(CList *list, size_t index);
bool CListAddAll(CList *dest, const CList *src);
bool CListAddAllDeep(CList *dest, const CList *src);
CList *CListCloneShallow(const CList *src);
CList *CListCloneDeep(const CList *src);
bool CListForEach(CList *list, CListVisitFunc visitFunc, void *context);
void CListClear(CList *list, bool freeData);
void CListPrint(const CList *list);
void CListFree(CList *list);

#endif
