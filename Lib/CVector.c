#include "CVector.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int CVectorGrow(CVector *vec, size_t min_capacity) {
    if (!vec) {
        return 0;
    }

    size_t new_capacity = vec->capacity ? vec->capacity : VECTOR_INIT_CAPACITY;
    while (new_capacity < min_capacity) {
        if (new_capacity > SIZE_MAX / 2) {
            return 0;
        }
        new_capacity *= 2;
    }

    if (vec->elem_size != 0 && new_capacity > SIZE_MAX / vec->elem_size) {
        return 0;
    }

    void *new_data = realloc(vec->data, new_capacity * vec->elem_size);
    if (!new_data) {
        return 0;
    }

    vec->data = new_data;
    vec->capacity = new_capacity;
    return 1;
}

CVector *CVectorInit(size_t elem_size,
                     void (*Destory)(void *),
                     int (*CMP)(const void *, const void *),
                     void (*Copy)(void *, const void *)) {
    if (elem_size == 0 || elem_size > SIZE_MAX / VECTOR_INIT_CAPACITY) {
        return NULL;
    }

    CVector *cVector = malloc(sizeof(CVector));
    if (!cVector) {
        return NULL;
    }

    cVector->size = 0;
    cVector->CMP = CMP;
    cVector->elem_size = elem_size;
    cVector->Destory = Destory;
    cVector->Copy = Copy;
    cVector->capacity = VECTOR_INIT_CAPACITY;
    cVector->data = malloc(cVector->capacity * elem_size);
    if (!cVector->data) {
        free(cVector);
        return NULL;
    }

    return cVector;
}

void CVectorDestroy(CVector *vec) {
    if (!vec) {
        return;
    }

    if (vec->Destory) {
        for (size_t i = 0; i < vec->size; i++) {
            void *data = CVectorAt(vec, i);
            if (data) {
                vec->Destory(data);
            }
        }
    }
    free(vec->data);
    free(vec);
}

void CVectorPushBack(CVector *vec, const void *value) {
    if (!vec || !value) {
        return;
    }

    if (vec->size + 1 > vec->capacity && !CVectorGrow(vec, vec->size + 1)) {
        return;
    }

    void *dest = (char *)vec->data + vec->size * vec->elem_size;
    if (vec->Copy) {
        vec->Copy(dest, value); // 深拷贝
    } else {
        memcpy(dest, value, vec->elem_size); // 浅拷贝
    }
    vec->size++;
}

void CVectorPopBack(CVector *vec) {
    if (!vec || vec->size == 0) {
        return;
    }

    vec->size--;
    void *elem = (char *)vec->data + vec->size * vec->elem_size;
    if (vec->Destory) {
        vec->Destory(elem);
    }
}

void *CVectorAt(CVector *vec, size_t index) {
    if (!vec || index >= vec->size) {
        return NULL;
    }
    return (char *)vec->data + index * vec->elem_size;
}

CVectorIterator CVectorBegin(CVector *vec) {
    CVectorIterator it;
    it.data = vec ? vec->data : NULL;
    it.elem_size = vec ? vec->elem_size : 0;
    return it;
}

CVectorIterator CVectorEnd(CVector *vec) {
    CVectorIterator it;
    it.data = vec ? (char *)vec->data + vec->size * vec->elem_size : NULL;
    it.elem_size = vec ? vec->elem_size : 0;
    return it;
}

CVectorIterator CVectorNext(CVector *vec, CVectorIterator it) {
    (void)vec;
    CVectorIterator next = it;
    if (next.data) {
        next.data = (char *)next.data + next.elem_size;
    }
    return next;
}

CVectorIterator CVectorPrev(CVector *vec, CVectorIterator it) {
    (void)vec;
    CVectorIterator prev = it;
    if (prev.data) {
        prev.data = (char *)prev.data - prev.elem_size;
    }
    return prev;
}

void CVectorInsert(CVector *vec, size_t index, const void *value) {
    if (!vec || !value || index > vec->size) {
        return;
    }

    // 扩容（按需）
    if (vec->size + 1 > vec->capacity && !CVectorGrow(vec, vec->size + 1)) {
        return;
    }
    // 将 index 及后面元素向后移动
    void *dst = (char *)vec->data + (index + 1) * vec->elem_size;
    void *src = (char *)vec->data + index * vec->elem_size;
    size_t move_size = (vec->size - index) * vec->elem_size;
    memmove(dst, src, move_size);

    void *dest = (char *)vec->data + index * vec->elem_size;
    if (vec->Copy) {
        vec->Copy(dest, value);
    } else {
        memcpy(dest, value, vec->elem_size);
    }
    vec->size++;
}

void CVectorClear(CVector *vec) {
    if (!vec) {
        return;
    }

    if (vec->Destory) {
        for (size_t i = 0; i < vec->size; ++i) {
            void *elem = (char *)vec->data + i * vec->elem_size;
            vec->Destory(elem);
        }
    }
    vec->size = 0;
}

void CVectorErase(CVector *vec, size_t index) {
    if (!vec || index >= vec->size) {
        return;
    }

    void *elem = (char *)vec->data + index * vec->elem_size;
    if (vec->Destory) {
        vec->Destory(elem);
    }

    // 后面元素前移
    if (index < vec->size - 1) {
        void *src = (char *)vec->data + (index + 1) * vec->elem_size;
        size_t move_size = (vec->size - index - 1) * vec->elem_size;
        memmove(elem, src, move_size);
    }

    vec->size--;
}

size_t CVectorFind(CVector *vec, const void *value) {
    if (!vec || !value) {
        return SIZE_MAX;
    }

    for (size_t i = 0; i < vec->size; ++i) {
        void *elem = (char *)vec->data + i * vec->elem_size;
        if (vec->CMP) {
            if (vec->CMP(elem, value) == 0) {
                return i;
            }
        } else if (memcmp(elem, value, vec->elem_size) == 0) {
            return i;
        }
    }
    return SIZE_MAX; // 找不到返回 -1（注意需特别处理 size_t 与 int 之间转换）
}

void CVectorSet(CVector *vec, size_t index, const void *value) {
    if (!vec || !value || index >= vec->size) {
        return;
    }

    void *dest = CVectorAt(vec, index);
    if (!dest) {
        return;
    }

    if (vec->Destory) {
        vec->Destory(dest); // 可选：先销毁原来的元素
    }
    if (vec->Copy) {
        vec->Copy(dest, value);
    } else {
        memcpy(dest, value, vec->elem_size);
    }
}

void CVectorResize(CVector *vec, size_t new_size, const void *default_value) {
    if (!vec) {
        return;
    }

    if (new_size > vec->capacity && !CVectorGrow(vec, new_size)) {
        return;
    }

    // 构造新元素
    for (size_t i = vec->size; i < new_size; ++i) {
        void *dest = (char *)vec->data + i * vec->elem_size;
        if (default_value) {
            if (vec->Copy) {
                vec->Copy(dest, default_value);
            } else {
                memcpy(dest, default_value, vec->elem_size);
            }
        } else {
            memset(dest, 0, vec->elem_size);
        }
    }

    // 销毁多余元素
    if (new_size < vec->size && vec->Destory) {
        for (size_t i = new_size; i < vec->size; ++i) {
            vec->Destory((char *)vec->data + i * vec->elem_size);
        }
    }

    vec->size = new_size;
}
