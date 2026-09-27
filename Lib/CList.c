#include "CList.h"

#include <stdio.h>
#include <stdlib.h>

CList *CListInit(
    CListFreeFunc freeFunc,
    CListEqualsFunc equalsFunc,
    CListPrintFunc printFunc
) {
    return CListInitWithCopy(freeFunc, equalsFunc, printFunc, NULL);
}

CList *CListInitWithCopy(
    CListFreeFunc freeFunc,
    CListEqualsFunc equalsFunc,
    CListPrintFunc printFunc,
    CListCopyFunc copyFunc
) {
    CList *list = malloc(sizeof(CList));
    if (!list) {
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    list->freeFunc = freeFunc;
    list->equalsFunc = equalsFunc;
    list->printFunc = printFunc;
    list->copyFunc = copyFunc;
    return list;
}

void CListSetCopyFunc(CList *list, CListCopyFunc copyFunc) {
    if (!list) {
        return;
    }
    list->copyFunc = copyFunc;
}

bool CListAppend(CList *list, void *data) {
    if (!list) {
        return false;
    }

    CListNode *node = malloc(sizeof(CListNode));
    if (!node) {
        return false;
    }

    node->data = data;
    node->next = NULL;

    if (!list->head) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }

    list->size++;
    return true;
}

bool CListAppendCopy(CList *list, const void *data) {
    if (!list || !list->copyFunc) {
        return false;
    }

    void *copy = list->copyFunc(data);
    if (data && !copy) {
        return false;
    }

    if (!CListAppend(list, copy)) {
        if (list->freeFunc) {
            list->freeFunc(copy);
        }
        return false;
    }

    return true;
}

void *CListGet(const CList *list, size_t index) {
    if (!list || index >= list->size) {
        return NULL;
    }

    CListNode *cur = list->head;
    for (size_t i = 0; cur && i < index; i++) {
        cur = cur->next;
    }

    return cur ? cur->data : NULL;
}

bool CListIsEmpty(const CList *list) {
    return !list || list->size == 0;
}

long CListIndexOf(const CList *list, const void *data) {
    if (!list || !list->equalsFunc) {
        return -1;
    }

    long index = 0;
    for (CListNode *cur = list->head; cur; cur = cur->next) {
        if (list->equalsFunc(cur->data, data)) {
            return index;
        }
        index++;
    }
    return -1;
}

bool CListContains(const CList *list, const void *data) {
    return CListIndexOf(list, data) >= 0;
}

bool CListRemoveByValue(CList *list, const void *data) {
    if (!list || !list->equalsFunc) {
        return false;
    }

    CListNode *cur = list->head;
    CListNode *prev = NULL;

    while (cur) {
        if (list->equalsFunc(cur->data, data)) {
            if (prev) {
                prev->next = cur->next;
            } else {
                list->head = cur->next;
            }
            if (list->tail == cur) {
                list->tail = prev;
            }

            if (list->freeFunc) {
                list->freeFunc(cur->data);
            }
            free(cur);
            list->size--;
            return true;
        }

        prev = cur;
        cur = cur->next;
    }

    return false;
}

void *CListRemoveByIndex(CList *list, size_t index) {
    if (!list || index >= list->size) {
        return NULL;
    }

    CListNode *cur = list->head;
    CListNode *prev = NULL;

    for (size_t i = 0; i < index; i++) {
        prev = cur;
        cur = cur->next;
    }

    if (prev) {
        prev->next = cur->next;
    } else {
        list->head = cur->next;
    }
    if (list->tail == cur) {
        list->tail = prev;
    }

    void *data = cur->data;
    free(cur);
    list->size--;
    return data;
}

bool CListAddAll(CList *dest, const CList *src) {
    if (!dest || !src) {
        return false;
    }

    /*
     * Shallow copy: only appends each data pointer from src.
     * Do not assign freeFunc to both lists for the same owned objects unless
     * duplicate frees are intentionally avoided elsewhere.
     */
    for (CListNode *cur = src->head; cur; cur = cur->next) {
        if (!CListAppend(dest, cur->data)) {
            return false;
        }
    }

    return true;
}

bool CListAddAllDeep(CList *dest, const CList *src) {
    if (!dest || !src || !dest->copyFunc) {
        return false;
    }

    size_t original_size = dest->size;
    for (CListNode *cur = src->head; cur; cur = cur->next) {
        if (!CListAppendCopy(dest, cur->data)) {
            while (dest->size > original_size) {
                void *data = CListRemoveByIndex(dest, original_size);
                if (dest->freeFunc) {
                    dest->freeFunc(data);
                }
            }
            return false;
        }
    }

    return true;
}

CList *CListCloneShallow(const CList *src) {
    if (!src) {
        return NULL;
    }

    /*
     * Shallow clones do not own element data. freeFunc is intentionally NULL
     * so the clone cannot double-free objects owned by the source list.
     */
    CList *clone = CListInitWithCopy(NULL, src->equalsFunc, src->printFunc, src->copyFunc);
    if (!clone) {
        return NULL;
    }

    if (!CListAddAll(clone, src)) {
        CListFree(clone);
        return NULL;
    }

    return clone;
}

CList *CListCloneDeep(const CList *src) {
    if (!src || !src->copyFunc) {
        return NULL;
    }

    CList *clone = CListInitWithCopy(src->freeFunc, src->equalsFunc, src->printFunc, src->copyFunc);
    if (!clone) {
        return NULL;
    }

    if (!CListAddAllDeep(clone, src)) {
        CListFree(clone);
        return NULL;
    }

    return clone;
}

bool CListForEach(CList *list, CListVisitFunc visitFunc, void *context) {
    if (!list || !visitFunc) {
        return false;
    }

    for (CListNode *cur = list->head; cur; cur = cur->next) {
        if (!visitFunc(cur->data, context)) {
            return false;
        }
    }

    return true;
}

void CListClear(CList *list, bool freeData) {
    if (!list) {
        return;
    }

    CListNode *cur = list->head;
    while (cur) {
        CListNode *next = cur->next;
        if (freeData && list->freeFunc) {
            list->freeFunc(cur->data);
        }
        free(cur);
        cur = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void CListPrint(const CList *list) {
    if (!list || !list->printFunc) {
        return;
    }

    printf("[");
    for (CListNode *cur = list->head; cur; cur = cur->next) {
        list->printFunc(cur->data);
        if (cur->next) {
            printf(", ");
        }
    }
    printf("]\n");
}

void CListFree(CList *list) {
    if (!list) {
        return;
    }

    CListClear(list, true);
    free(list);
}
