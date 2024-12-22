//
// Created by yaohuayuan on 2024/9/18.
//

#ifndef DBMS_C_PAGE_H
#define DBMS_C_PAGE_H

#include "../head/ByteBuffer.h"
#include <stdio.h>

/**
 * 定义 Page 结构体，用于表示数据库管理系统中的一个页面。
 * 页面是数据存储的基本单位，通常对应于磁盘上的固定大小的数据块。
 */
typedef struct {
    ByteBuffer *buffer; // 页面内容的实际存储，使用 ByteBuffer 来管理字节数据。
} Page;

/**
 * 初始化一个新的 Page 实例，并分配指定大小的缓冲区。
 *
 * @param size 要分配给页面的缓冲区大小（以字节为单位）。
 * @return 返回初始化后的 Page 指针。
 */
Page *PageInit(int size);

/**
 * 使用已有的 ByteBuffer 初始化一个新的 Page 实例。
 *
 * @param buffer 已存在的 ByteBuffer 对象指针。
 * @return 返回初始化后的 Page 指针。
 */
Page *PageInitBytes(ByteBuffer *buffer);

/**
 * 从页面中读取一个整数（int 类型），并将其写入到 ByteBufferData 中。
 *
 * @param p 指向 Page 的指针。
 * @param position 数据在页面中的偏移量。
 * @param output 指向 ByteBufferData 的指针，用于接收读取的数据。
 * @return 返回读取操作的状态码或结果。
 */
int PageGetInt(Page *p, int position, ByteBufferData *output);

/**
 * 在页面中设置一个整数（int 类型）。
 *
 * @param p 指向 Page 的指针。
 * @param position 数据在页面中的偏移量。
 * @param data 要写入的整数值。
 */
void PageSetInt(Page *p, int position, int data);

/**
 * 从页面中读取一个短整数（short 类型）。
 *
 * @param p 指向 Page 的指针。
 * @param position 数据在页面中的偏移量。
 * @return 返回读取的短整数值。
 */
int PageGetShot(Page *p, int position);

/**
 * 从页面中读取一个长整数（long 类型）。
 *
 * @param p 指向 Page 的指针。
 * @param position 数据在页面中的偏移量。
 * @return 返回读取的长整数值。
 */
int PageGetLong(Page *p, int position);

/**
 * 从页面中读取一个字符串。
 *
 * @param p 指向 Page 的指针。
 * @param position 数据在页面中的偏移量。
 * @return 返回读取的字符串。
 */
char *PageGetString(Page *p, int position);

/**
 * 在页面中设置一个字符串。
 *
 * @param p 指向 Page 的指针。
 * @param position 数据在页面中的偏移量。
 * @param str 要写入的字符串。
 */
void PageSetString(Page *p, int position, const char *str);

/**
 * 获取页面的内容作为 ByteBuffer。
 *
 * @param p 指向 Page 的指针。
 * @param position 数据在页面中的偏移量。
 * @return 返回指向页面内容的 ByteBuffer 指针。
 */
ByteBuffer *PageContent(Page *p, int position);

/**
 * 在页面中设置一段原始字节数组。
 *
 * @param p 指向 Page 的指针。
 * @param position 数据在页面中的偏移量。
 * @param data 要写入的原始字节数组。
 * @param type 数据类型信息（可能用于序列化/反序列化）。
 * @param size 数据长度（以字节为单位）。
 */
void PageSetBytes(Page *p, int position, const uint8_t *data, uint8_t *type, uint64_t size);

/**
 * 从页面中读取一段原始字节数组。
 *
 * @param p 指向 Page 的指针。
 * @param position 数据在页面中的偏移量。
 * @return 返回包含读取数据的 ByteBufferData。
 */
ByteBufferData *PageGetBytes(Page *p, int position);

/**
 * 计算给定字符串长度的最大存储空间需求。
 *
 * @param strlen 字符串的长度。
 * @return 返回所需的存储空间大小（以字节为单位）。
 */
int PageMaxLength(int strlen);

#endif //DBMS_C_PAGE_H