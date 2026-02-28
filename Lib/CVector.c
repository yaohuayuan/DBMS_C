//
// Created by yaohuayuan on 2025/5/30.
//


#include <memory.h>
#include <assert.h>
#include <string.h>
#include "CVector.h"
#include "malloc.h"
#include "CCMath.h"
CVector *CVectorInit(size_t elem_size,void (*Destory)(void*),int (*CMP)(const void*,const void *),void(*Copy)(void*,const void*)){
    CVector *cVector = malloc(sizeof(CVector));
    cVector->size = 0;
    cVector->CMP = CMP;
    cVector->elem_size = elem_size;
    cVector->Destory = Destory;
    cVector->Copy = Copy;
    cVector->capacity =  VECTOR_INIT_CAPACITY;
    cVector->data = malloc(cVector->capacity * elem_size);

    return cVector;
}
void CVectorDestroy(CVector* vec){
    if(vec->Destory){
        for(int i=0;i<vec->size;i++) {
            void *data = CVectorAt(vec, i);
            vec->Destory(data);
        }
    }
    free(vec->data);
    free(vec);
}
void CVectorPushBack(CVector* vec, const void* value){
    if(vec->size+1>vec->capacity){
        void * data = realloc(vec->data,vec->elem_size * vec->capacity * 2);
        if(!data){
            perror("CVector Error:Data empty");
            return;
        }
        vec->data = data;
        vec->capacity = vec->capacity*2;
    }
    void* dest = (char*)vec->data + vec->size * vec->elem_size;

    if (vec->Copy)
        vec->Copy(dest, value);   // 深拷贝
    else
        memcpy(dest, value, vec->elem_size);  // 浅拷贝
    vec->size++;
}
void CVectorPopBack(CVector* vec){
    if(vec->size==0){
        perror("CVector Error,PopBack But Size==0");
        return ;
    }
    vec->size--;
    void* elem = (char*)vec->data + vec->size * vec->elem_size;
    if (vec->Destory)
        vec->Destory(elem);
}
void* CVectorAt(CVector* vec, size_t index){
    if(vec==NULL|| index >= vec->size)
        return NULL;
    return (char*)vec->data + index * vec->elem_size;
}
CVectorIterator CVectorBegin(CVector* vec) {
    CVectorIterator it;
    it.data = vec->data;            // 指向首元素
    it.elem_size = vec->elem_size;
    return it;
}

CVectorIterator CVectorEnd(CVector* vec) {
    CVectorIterator it;
    it.data = (char*)vec->data + vec->size * vec->elem_size;  // 超尾地址
    it.elem_size = vec->elem_size;
    return it;
}

CVectorIterator CVectorNext(CVector* vec, CVectorIterator it) {
    CVectorIterator next = it;
    next.data = (char*)it.data + it.elem_size;
    return next;
}

CVectorIterator CVectorPrev(CVector* vec, CVectorIterator it) {
    CVectorIterator prev = it;
    prev.data = (char*)it.data - it.elem_size;
    return prev;
}
void CVectorInsert(CVector* vec, size_t index, const void* value){
    assert(vec && value);
    assert(index <= vec->size);  // 插入位置可以等于 size（尾插）

    // 扩容（按需）
    if (vec->size >= vec->capacity) {
        size_t new_capacity = vec->capacity ? vec->capacity * 2 : 4;
        void* new_data = realloc(vec->data, new_capacity * vec->elem_size);
        assert(new_data);
        vec->data = new_data;
        vec->capacity = new_capacity;
    }

    // 将 index 及后面元素向后移动
    void* dst = (char*)vec->data + (index + 1) * vec->elem_size;
    void* src = (char*)vec->data + index * vec->elem_size;
    size_t move_size = (vec->size - index) * vec->elem_size;
    memmove(dst, src, move_size);

    void* dest = (char*)vec->data + index * vec->elem_size;

    if (vec->Copy)
        vec->Copy(dest, value);
    else
        memcpy(dest, value, vec->elem_size);

    vec->size++;
}
void CVectorClear(CVector* vec) {
    assert(vec);
    if (vec->Destory) {
        for (size_t i = 0; i < vec->size; ++i) {
            void* elem = (char*)vec->data + i * vec->elem_size;
            vec->Destory(elem);
        }
    }
    vec->size = 0;
}
void CVectorErase(CVector* vec, size_t index) {
    assert(vec && index < vec->size);

    void* elem = (char*)vec->data + index * vec->elem_size;
    if (vec->Destory) {
        vec->Destory(elem);
    }

    // 后面元素前移
    if (index < vec->size - 1) {
        void* dst = elem;
        void* src = (char*)vec->data + (index + 1) * vec->elem_size;
        size_t move_size = (vec->size - index - 1) * vec->elem_size;
        memmove(dst, src, move_size);
    }

    vec->size--;
}
size_t CVectorFind(CVector* vec, const void* value) {
    assert(vec && value );

    for (size_t i = 0; i < vec->size; ++i) {
        void* elem = (char*)vec->data + i * vec->elem_size;
        if (vec->CMP){
            if (vec->CMP(elem, value) == 0) {
                return i;
            }
        }else{
            if (memcmp(elem, value, vec->elem_size) == 0)
                return i;
        }

    }
    return SIZE_MAX; // 找不到返回 -1（注意需特别处理 size_t 与 int 之间转换）
}
void CVectorSet(CVector* vec, size_t index, const void* value){
    assert(vec && index < vec->size);
    void* dest = CVectorAt(vec, index);
    assert(dest);
    if (vec->Destory)
        vec->Destory(dest);  // 可选：先销毁原来的元素
    if (vec->Copy)
        vec->Copy(dest, value);
    else
        memcpy(dest, value, vec->elem_size);
}
void CVectorResize(CVector* vec, size_t new_size, const void* default_value) {
    assert(vec);

    if (new_size > vec->capacity) {
        size_t cap = vec->capacity;
        while (cap < new_size) cap *= 2;
        void* new_data = realloc(vec->data, cap * vec->elem_size);
        assert(new_data);
        vec->data = new_data;
        vec->capacity = cap;
    }

    // 构造新元素
    for (size_t i = vec->size; i < new_size; ++i) {
        void* dest = (char*)vec->data + i * vec->elem_size;
        if (default_value) {
            if (vec->Copy) vec->Copy(dest, default_value);
            else memcpy(dest, default_value, vec->elem_size);
        } else {
            memset(dest, 0, vec->elem_size);
        }
    }

    // 销毁多余元素
    if (new_size < vec->size && vec->Destory) {
        for (size_t i = new_size; i < vec->size; ++i) {
            vec->Destory((char*)vec->data + i * vec->elem_size);
        }
    }

    vec->size = new_size;
}
