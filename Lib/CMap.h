//
// Created by Lenovo on 2025/6/24.
//

#ifndef NEWDBMS_CMAP_H
#define NEWDBMS_CMAP_H
#include <stddef.h> // For size_t
#include "RBT.h"
typedef struct {
    RBTree tree;
    size_t key_size;
    size_t value_size;
    void* (*DeepCopyKey)(void* src);
    void* (*DeepCopyData)(void* src);
} CMap;
/**
 * @brief 初始化一个Map。
 *
 * @param map 指向要初始化的CMap结构体。
 * @param keySize 键类型的大小（字节）。
 * @param valueSize 值类型的大小（字节）。
 * @param compare 用于比较两个键的函数指针。
 * @param keyFree 用于释放键内存的函数指针（可为NULL）。
 * @param valueFree 用于释放值内存的函数指针（可为NULL）。
 * @return 成功返回1，失败返回0。
 */
int CMapInit(CMap *map,
            size_t keySize,
            size_t valueSize,
            int (*compare)(const void*, const void*),
            void (*keyFree)(void*),
            void (*valueFree)(void*),
            void* (*deepCopyKey)(void*),
            void* (*deepCopyData)(void*)
            )
            ;
/**
 * @brief 向Map中插入一个键值对。复制键和值数据。
 *
 * @param map 指向Map的指针。
 * @param key 指向键数据的指针。
 * @param value 指向值数据的指针。
 * @return 成功返回1，失败返回0（例如，内存分配错误）。
 */
int CMapInsert(CMap *map, const void *key, const void *value);

/**
 * @brief 在Map中查找与给定键关联的值。
 *
 * @param map 指向Map的指针。
 * @param key 指向要查找的键的指针。
 * @return 如果找到，返回指向值的指针；否则，返回NULL。
 */
void* CMapFind(CMap *map, const void *key);

/**
 * @brief 从Map中删除一个键值对。
 *
 * @param map 指向Map的指针。
 * @param key 指向要删除的键的指针。
 * @return 成功返回1，如果未找到键则返回0。
 */
int CMapErase(CMap *map, const void *key);

/**
 * @brief 修改Map中已存在键的值。如果键不存在，返回0。
 *
 * @param map 指向Map的指针。
 * @param key 指向键的指针。
 * @param new_value 指向新值的指针。
 * @return 如果键存在并成功修改返回1，失败或键不存在返回0。
 */
int CMapUpdate(CMap *map, const void *key, const void *new_value);

/**
 * @brief 释放Map关联的所有内存。
 *
 * @param map 指向Map的指针。
 */

void CMapDestroy(CMap *map);
/**
 * @brief CMap迭代器结构体
 */
typedef struct {
    RBNode *node;   // 当前节点
    RBTree *tree;   // 红黑树
} CMapIterator;

/**
 * @brief 获取CMap中第一个元素的迭代器。
 *
 * @param map 指向CMap结构体的指针。
 * @return CMapIterator 迭代器。
 */
CMapIterator CMapIteratorBegin(CMap *map);

/**
 * @brief 获取CMap中结束迭代器。
 *
 * @param map 指向CMap结构体的指针。
 * @return CMapIterator 结束迭代器。
 */
CMapIterator CMapIteratorEnd(CMap *map);

/**
 * @brief 获取当前迭代器所指向节点的键。
 *
 * @param it 迭代器。
 * @return 键。
 */
void* CMapIteratorKey(CMapIterator *it);

/**
 * @brief 获取当前迭代器所指向节点的值。
 *
 * @param it 迭代器。
 * @return 值。
 */
void* CMapIteratorValue(CMapIterator *it);

/**
 * @brief 移动迭代器到下一个节点。
 *
 * @param it 迭代器。
 */
void CMapIteratorNext(CMapIterator *it);

/**
 * @brief 判断两个迭代器是否相等（是否指向同一节点）。
 *
 * @param it1 迭代器1。
 * @param it2 迭代器2。
 * @return 如果两个迭代器相等返回1，反之返回0。
 */
int CMapIteratorEqual(CMapIterator *it1, CMapIterator *it2);

/**
 * @brief 向Map中插入或更新一个键值对。
 *
 * @param map 指向Map的指针。
 * @param key 指向键数据的指针。
 * @param value 指向值数据的指针。
 * @return 成功返回1，失败返回0。
 */
int CMapPut(CMap *map, const void *key, const void *value) ;
#endif //NEWDBMS_CMAP_H
