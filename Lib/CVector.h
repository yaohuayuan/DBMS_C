//
// Created by yaohuayuan on 2025/5/30.
//
#ifndef NEWDBMS_CVECTOR_H
#define NEWDBMS_CVECTOR_H

#include <stddef.h>  // for size_t



#define VECTOR_INIT_CAPACITY 32
/**
 * 通用的动态数组结构，支持任意类型。
 */
typedef struct CVector {
    void* data;             // 元素存储区
    size_t size;            // 当前元素个数
    size_t capacity;        // 当前容量（元素个数）
    size_t elem_size;       // 每个元素的大小（单位：字节）
    void (*Destory)(void*);
    int (*CMP)(const void*,const void *);
    void (*Copy)(void*dest,const void*src);

} CVector;

/**
 * 迭代器类型，实质上是 void* 指针。
 */
typedef struct {
    void*data;
    size_t elem_size;
}CVectorIterator;

/**
 * 初始化一个新的 CVector。
 *
 * @param elem_size 元素大小（以字节为单位）。
 * @param Destory 元素的销毁函数。
 * @param CMP 元素的比较函数。
 * @param Copy 元素的复制函数。
 * @return 返回初始化后的 CVector 指针。
 */
CVector *CVectorInit(size_t elem_size,void (*Destory)(void*),int (*CMP)(const void*,const void *),void (*Copy)(void*,const void*));

/**
 * 销毁 CVector，释放内存。
 *
 * @param vec 指向要销毁的 CVector 对象。
 */
void CVectorDestroy(CVector* vec);

/**
 * 在尾部添加一个元素。
 *
 * @param vec 指向 CVector。
 * @param value 指向要添加的元素数据。
 */
void CVectorPushBack(CVector* vec, const void* value);

/**
 * 删除最后一个元素。
 *
 * @param vec 指向 CVector。
 */
void CVectorPopBack(CVector* vec);

/**
 * 获取指定索引处的元素指针。
 *
 * @param vec 指向 CVector。
 * @param index 元素索引。
 * @return 返回对应元素的指针。
 */
void* CVectorAt(CVector* vec, size_t index);

/**
 * 获取第一个元素的迭代器。
 *
 * @param vec 指向 CVector。
 * @return 返回起始位置的迭代器。
 */
CVectorIterator CVectorBegin(CVector* vec);

/**
 * 获取超尾元素迭代器（即末尾之后的位置）。
 *
 * @param vec 指向 CVector。
 * @return 返回末尾迭代器。
 */
CVectorIterator CVectorEnd(CVector* vec);

/**
 * 向前移动一个元素迭代器。
 *
 * @param vec 指向 CVector。
 * @param it 当前迭代器。
 * @return 返回前一个迭代器。
 */
CVectorIterator CVectorPrev(CVector* vec, CVectorIterator it);

/**
 * 向后移动一个元素迭代器。
 *
 * @param vec 指向 CVector。
 * @param it 当前迭代器。
 * @return 返回后一个迭代器。
 */
CVectorIterator CVectorNext(CVector* vec, CVectorIterator it);


/**
 * 清空 CVector 中的所有元素。
 *
 * @param vec 指向 CVector。
 */
void CVectorClear(CVector* vec);

/**
 * 在指定位置插入一个元素。
 *
 * @param vec 指向 CVector。
 * @param index 插入的位置索引。
 * @param value 指向插入的值。
 */
void CVectorInsert(CVector* vec, size_t index, const void* value);

/**
 * 删除指定位置的元素。
 *
 * @param vec 指向 CVector。
 * @param index 要删除的元素索引。
 */
void CVectorErase(CVector* vec, size_t index);

/**
 * 查找第一个与 value 相等的元素索引。
 *
 * @param vec 指向 CVector。
 * @param value 指向要查找的值。
 * @return 若找到，返回对应索引；否则返回 SIZE_MAX。
 */
size_t CVectorFind(CVector* vec, const void* value);

/**
 * 设置指定索引位置的元素值。
 *
 * @param vec 指向 CVector。
 * @param index 要设置的元素索引。
 * @param value 指向新的元素值。
 */
void CVectorSet(CVector* vec, size_t index, const void* value);

/**
 * 调整 CVector 的大小。
 *
 * @param vec 指向 CVector。
 * @param new_size 新的大小。
 * @param default_value 指向新添加元素的默认值（如果需要）。
 */
void CVectorResize(CVector* vec, size_t new_size, const void* default_value);
#endif //NEWDBMS_CVECTOR_H
