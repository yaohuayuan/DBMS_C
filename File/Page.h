//
// Created by yaohuayuan on 2025/5/31.
//

#ifndef NEWDBMS_PAGE_H
#define NEWDBMS_PAGE_H
#include "ByteBuffer.h"
#include "CString.h"
typedef struct Page{
    ByteBuffer *buffer;
}Page;

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
int PageGetInt(Page *p, int position);

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
short PageGetShort(Page *p, int position);

/**
 * 从页面中读取一个长整数（long 类型）。
 *
 * @param p 指向 Page 的指针。
 * @param position 数据在页面中的偏移量。
 * @return 返回读取的长整数值。
 */
long long PageGetLong(Page *p, int position);

/**
 * 从页面中读取一个字符串。
 *
 * @param p 指向 Page 的指针。
 * @param position 数据在页面中的偏移量。
 * @return 返回读取的字符串。
 */
CString *PageGetString(Page *p, int position);

/**
 * 在页面中设置一个字符串。
 *
 * @param p 指向 Page 的指针。
 * @param position 数据在页面中的偏移量。
 * @param str 要写入的字符串。
 */
void PageSetString(Page *p, int position, const CString *str);


/**
 * 在页面中设置一段原始字节数组。
 *
 * @param p 指向 Page 的指针。
 * @param position 数据在页面中的偏移量。
 * @param data 要写入的原始字节数组。
 * @param size 数据长度（以字节为单位）。
 */
void PageSetBytes(Page *p, int position, const uint8_t *data,  uint64_t size);

/**
 * 从页面中读取一段原始字节数组。
 *
 * @param p 指向 Page 的指针。
 * @param position 数据在页面中的偏移量。
 * @return 返回指向读取数据的指针。
 */
uint8_t *PageGetBytes(Page *p, int position);

/**
 * 计算给定字符串所需的最大存储空间。
 *
 * @param cString 要计算的字符串。
 * @return 返回所需的存储空间大小（以字节为单位）。
 */
int PageMaxLength(CString*cString);

/**
 * 销毁 Page 实例并释放其资源。
 *
 * @param p 指向要销毁的 Page 的指针。
 */
void PageDestroy(Page *p);

/**
 * 直接在页面中设置一段原始字节数组，不进行任何格式转换。
 *
 * @param p 指向 Page 的指针。
 * @param pos 数据在页面中的偏移量。
 * @param data 要写入的原始字节数组。
 * @param size 数据长度（以字节为单位）。
 */
void PageSetBytesRaw(Page* p, int pos, const uint8_t* data, uint32_t size);

/**
 * 直接从页面中读取一段原始字节数组，不进行任何格式转换。
 *
 * @param p 指向 Page 的指针。
 * @param pos 数据在页面中的偏移量。
 * @param out 用于存储读取数据的缓冲区。
 * @param size 要读取的数据长度（以字节为单位）。
 */
void PageGetBytesRaw(Page* p, int pos, uint8_t* out, uint32_t size);

#endif //NEWDBMS_PAGE_H
