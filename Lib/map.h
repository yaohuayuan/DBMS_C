//
// Created by yaohuayuan on 2024/10/24.
//
/**
 * Copyright (c) 2014 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#ifndef DBMS_C_MAP_H
#define DBMS_C_MAP_H

#include <stdlib.h>
#include <string.h>

#define MAP_VERSION "0.1.0"

/// @brief 链表节点结构体定义。
struct map_node_t;
typedef struct map_node_t map_node_t;

/**
 * @brief 哈希表基础结构体定义。
 */
typedef struct {
    map_node_t **buckets; ///< 桶数组，每个桶指向一个链表头节点
    unsigned nbuckets;    ///< 桶的数量
    unsigned nnodes;      ///< 节点总数
} map_base_t;

/**
 * @brief 迭代器结构体定义，用于遍历哈希表。
 */
typedef struct {
    unsigned bucketidx;   ///< 当前迭代的桶索引
    map_node_t *node;     ///< 当前迭代的节点指针
} map_iter_t;

/**
 * @brief 定义一个通用类型的哈希表。
 *
 * @param T 数据类型
 */
#define map_t(T)\
  struct { map_base_t base; T *ref; T tmp; }

/**
 * @brief 初始化哈希表实例。
 *
 * @param m 指向哈希表实例的指针
 */
#define map_init(m)\
  memset(m, 0, sizeof(*(m)))

/**
 * @brief 释放哈希表实例及其所有节点占用的内存。
 *
 * @param m 指向哈希表实例的指针
 */
#define map_deinit(m)\
  map_deinit_(&(m)->base)

/**
 * @brief 获取哈希表中指定键对应的值。
 *
 * @param m 指向哈希表实例的指针
 * @param key 键名
 */
#define map_get(m, key)\
  ( (m)->ref = map_get_(&(m)->base, key) )

/**
 * @brief 向哈希表中设置键值对。
 *
 * @param m 指向哈希表实例的指针
 * @param key 键名
 * @param value 键值
 */
#define map_set(m, key, value)\
  ( (m)->tmp = (value),\
    map_set_(&(m)->base, key, &(m)->tmp, sizeof((m)->tmp)) )

/**
 * @brief 从哈希表中移除指定键的键值对。
 *
 * @param m 指向哈希表实例的指针
 * @param key 键名
 */
#define map_remove(m, key)\
  map_remove_(&(m)->base, key)

/**
 * @brief 创建一个新的迭代器实例。
 *
 * @param m 指向哈希表实例的指针
 */
#define map_iter(m)\
  map_iter_()

/**
 * @brief 获取迭代器的下一个键值对。
 *
 * @param m 指向哈希表实例的指针
 * @param iter 指向迭代器实例的指针
 */
#define map_next(m, iter)\
  map_next_(&(m)->base, iter)

/**
 * @brief 释放哈希表基础结构体及其所有节点占用的内存。
 *
 * @param m 指向哈希表基础结构体的指针
 */
void map_deinit_(map_base_t *m);

/**
 * @brief 获取哈希表中指定键对应的值。
 *
 * @param m 指向哈希表基础结构体的指针
 * @param key 键名
 * @return 返回键对应的值指针；如果未找到，则返回 NULL。
 */
void *map_get_(map_base_t *m, const char *key);

/**
 * @brief 向哈希表中设置键值对。
 *
 * @param m 指向哈希表基础结构体的指针
 * @param key 键名
 * @param value 键值指针
 * @param vsize 键值大小（字节数）
 * @return 如果插入成功，返回 1；否则返回 0。
 */
int map_set_(map_base_t *m, const char *key, void *value, int vsize);

/**
 * @brief 从哈希表中移除指定键的键值对。
 *
 * @param m 指向哈希表基础结构体的指针
 * @param key 键名
 */
void map_remove_(map_base_t *m, const char *key);

/**
 * @brief 创建一个新的迭代器实例。
 *
 * @return 返回初始化后的迭代器实例。
 */
map_iter_t map_iter_(void);

/**
 * @brief 获取迭代器的下一个键值对。
 *
 * @param m 指向哈希表基础结构体的指针
 * @param iter 指向迭代器实例的指针
 * @return 返回下一个键名；如果遍历结束，则返回 NULL。
 */
const char *map_next_(map_base_t *m, map_iter_t *iter);

/// @brief 定义存储任意类型数据的哈希表类型。
typedef map_t(void*) map_void_t;

/// @brief 定义存储字符串类型数据的哈希表类型。
typedef map_t(char*) map_str_t;

/// @brief 定义存储整数类型数据的哈希表类型。
typedef map_t(int) map_int_t;

/// @brief 定义存储字符类型数据的哈希表类型。
typedef map_t(char) map_char_t;

/// @brief 定义存储浮点数类型数据的哈希表类型。
typedef map_t(float) map_float_t;

/// @brief 定义存储双精度浮点数类型数据的哈希表类型。
typedef map_t(double) map_double_t;

#endif //DBMS_C_MAP_H