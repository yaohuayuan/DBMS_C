//
// Created by yaohuayuan on 2025/5/31.
//

#ifndef NEWDBMS_BYTEBUFFER_H
#define NEWDBMS_BYTEBUFFER_H
#include <stdint.h>
#include <malloc.h>
#include "CString.h"

typedef struct ByteBuffer{
    uint8_t *data;
    uint64_t size;
    uint64_t position;
    uint64_t limit;
}ByteBuffer;
typedef enum {
    BYTEBUFFER_OK = 0,        ///< 操作成功
    BYTEBUFFER_ERROR_NULL,    ///< 空指针错误
    BYTEBUFFER_ERROR_BOUNDS,  ///< 越界访问错误
} ByteBufferError;

/**
 * @brief 获取 ByteBufferError 对应的错误信息字符串。
 *
 * @param err 错误代码。
 * @return 返回错误信息字符串。
 */
const char* byteBufferErrorString(ByteBufferError err);

/**
 * @brief 初始化一个新的 ByteBuffer 实例，不分配任何内存。
 *
 * @return 返回指向初始化的 ByteBuffer 的指针；如果初始化失败，返回 NULL。
 */
ByteBuffer* byte_buffer_init();

/**
 * @brief 分配一个新的 ByteBuffer 实例，并为其分配指定大小的内存。
 *
 * @param size 缓冲区的大小（以字节为单位）。
 * @return 返回指向分配的 ByteBuffer 的指针；如果分配失败，返回 NULL。
 */
ByteBuffer* bufferAllocate(uint64_t size);

/**
 * @brief 清除缓冲区，将其所有字节设置为零，并将 position 设为 0，将 limit 设为缓冲区的大小。
 *
 * @param buffer 指向要清除的 ByteBuffer 的指针。
 */
void bufferClear(ByteBuffer* buffer);

/**
 * @brief 释放 ByteBuffer 实例及其数据内存。
 *
 * @param buffer 指向要释放的 ByteBuffer 的指针。
 */
void bufferFree(ByteBuffer* buffer);

/**
 * @brief 压缩缓冲区，将当前位置之后的数据移动到缓冲区的开始位置，并更新 position 和 limit。
 *
 * @param buffer 指向要压缩的 ByteBuffer 的指针。
 */
void bufferCompact(ByteBuffer* buffer);

/**
 * @brief 翻转缓冲区，将 limit 设为当前 position，并将 position 设为 0。
 *
 * @param buffer 指向要翻转的 ByteBuffer 的指针。
 */
void bufferFlip(ByteBuffer* buffer);

/**
 * @brief 将一个字节数据写入缓冲区当前位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param data 要写入的数据（一个字节）。
 * @return 如果写入成功，返回 BYTEBUFFER_OK；否则返回相应的错误代码。
 */
ByteBufferError bufferPut(ByteBuffer* buffer, uint8_t data);

/**
 * @brief 将一个 16 位的整数数据写入缓冲区当前位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param data 要写入的数据（16 位整数）。
 * @return 如果写入成功，返回 BYTEBUFFER_OK；否则返回相应的错误代码。
 */
ByteBufferError bufferPutShort(ByteBuffer* buffer, int16_t data);

/**
 * @brief 将一个 32 位的整数数据写入缓冲区当前位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param data 要写入的数据（32 位整数）。
 * @return 如果写入成功，返回 BYTEBUFFER_OK；否则返回相应的错误代码。
 */
ByteBufferError bufferPutInt(ByteBuffer* buffer, int32_t data);

/**
 * @brief 将一个 64 位的整数数据写入缓冲区当前位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param data 要写入的数据（64 位整数）。
 * @return 如果写入成功，返回 BYTEBUFFER_OK；否则返回相应的错误代码。
 */
ByteBufferError bufferPutLong(ByteBuffer* buffer, int64_t data);

/**
 * @brief 从缓冲区当前位置读取一个字节数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param output 指向用于存储读取数据的指针。
 * @return 如果读取成功，返回 BYTEBUFFER_OK；否则返回相应的错误代码。
 */
ByteBufferError bufferGet(ByteBuffer* buffer, uint8_t *output);

/**
 * @brief 从缓冲区当前位置读取一个 16 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param output 指向用于存储读取数据的指针。
 * @return 如果读取成功，返回 BYTEBUFFER_OK；否则返回相应的错误代码。
 */
ByteBufferError bufferGetShort(ByteBuffer* buffer, uint16_t *output);

/**
 * @brief 从缓冲区当前位置读取一个 32 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param output 指向用于存储读取数据的指针。
 * @return 如果读取成功，返回 BYTEBUFFER_OK；否则返回相应的错误代码。
 */
ByteBufferError bufferGetInt(ByteBuffer* buffer, int32_t *output);

/**
 * @brief 从缓冲区当前位置读取一个 64 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param output 指向用于存储读取数据的指针。
 * @return 如果读取成功，返回 BYTEBUFFER_OK；否则返回相应的错误代码。
 */
ByteBufferError bufferGetLong(ByteBuffer* buffer, uint64_t *output);

/**
 * @brief 从缓冲区当前位置读取指定数量的字节数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param output 指向用于存储读取数据的指针。
 * @param size 要读取的数据字节数。
 * @return 如果读取成功，返回 BYTEBUFFER_OK；否则返回相应的错误代码。
 */
ByteBufferError bufferGetBytes(ByteBuffer* buffer, uint8_t *output, int size);

/**
 * @brief 将一个字节数据写入缓冲区的指定位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 写入字节的位置。
 * @param data 要写入的数据（一个字节）。
 * @return 如果写入成功，返回 1；否则返回 0。
 */
ByteBufferError bufferPutPosition(ByteBuffer* buffer, uint64_t position, uint8_t data);

/**
 * @brief 将一个 16 位的整数数据写入缓冲区的指定位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 写入数据的位置。
 * @param data 要写入的数据（16 位整数）。
 * @return 如果写入成功，返回 2；否则返回 0。
 */
ByteBufferError bufferPutShortPosition(ByteBuffer* buffer, uint64_t position, int16_t data);

/**
 * @brief 将一个 32 位的整数数据写入缓冲区的指定位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 写入数据的位置。
 * @param data 要写入的数据（32 位整数）。
 * @return 如果写入成功，返回 4；否则返回 0。
 */
ByteBufferError bufferPutIntPosition(ByteBuffer* buffer, uint64_t position, int32_t data);

/**
 * @brief 将一个 64 位的整数数据写入缓冲区的指定位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 写入数据的位置。
 * @param data 要写入的数据（64 位整数）。
 * @return 如果写入成功，返回 8；否则返回 0。
 */
ByteBufferError bufferPutLongPosition(ByteBuffer* buffer, uint64_t position, int64_t data);

/**
 * @brief 从缓冲区的指定位置读取一个字节数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 读取字节的位置。
 * @param output 指向用于存储读取数据的 ByteBufferError 指针。
 * @return 如果读取成功，返回 1；否则返回 0。
 */
ByteBufferError bufferGetPosition(ByteBuffer* buffer, uint64_t position, uint8_t *output);

/**
 * @brief 从缓冲区的指定位置读取一个 16 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 读取数据的位置。
 * @param output 指向用于存储读取数据的 ByteBufferError 指针。
 * @return 如果读取成功，返回 2；否则返回 0。
 */
ByteBufferError bufferGetShortPosition(ByteBuffer* buffer, uint64_t position, uint16_t *output);

/**
 * @brief 从缓冲区的指定位置读取一个 32 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 读取数据的位置。
 * @param output 指向用于存储读取数据的 ByteBufferError 指针。
 * @return 如果读取成功，返回 4；否则返回 0。
 */
ByteBufferError bufferGetIntPosition(ByteBuffer* buffer, uint64_t position, int32_t *output);

/**
 * @brief 从缓冲区的指定位置读取一个 64 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 读取数据的位置。
 * @param output 指向用于存储读取数据的 ByteBufferError 指针。
 * @return 如果读取成功，返回 8；否则返回 0。
 */
ByteBufferError bufferGetLongPosition(ByteBuffer* buffer, uint64_t position, uint64_t *output);

/**
 * @brief 从缓冲区的指定位置读取指定数量的字节数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 读取数据的起始位置。
 * @param output 指向用于存储读取数据的 ByteBufferError 指针。
 * @param size 要读取的数据字节数。
 * @return 成功读取的字节数；如果读取失败，返回 0。
 */
ByteBufferError bufferGetBytesPosition(ByteBuffer* buffer, uint64_t position, uint8_t *output,int size);

/**
 * @brief 改变缓冲区的 position。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param newPosition 新的 position。
 * @return 返回成功改变的 position。
 */
uint64_t bufferPosition(ByteBuffer* buffer, uint64_t newPosition);

/**
 * @brief 将一段字节数组写入缓冲区的指定位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 写入数据的位置。
 * @param data 要写入的数据（字节数组）。
 * @param size 数据长度（字节数）。
 * @return 如果写入成功，返回 BYTEBUFFER_OK；否则返回相应的错误代码。
 */
ByteBufferError bufferPutBytesPosition(ByteBuffer* buffer, uint64_t position, uint8_t* data, uint64_t size);

/**
 * @brief 将一段字节数组写入缓冲区当前位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param data 要写入的数据（字节数组）。
 * @param size 数据长度（字节数）。
 * @return 如果写入成功，返回 BYTEBUFFER_OK；否则返回相应的错误代码。
 */
ByteBufferError bufferPutBytes(ByteBuffer* buffer, uint8_t* data, uint64_t size);

/**
 * @brief 使用指定大小和数据初始化一个新的 ByteBuffer 实例。
 *
 * @param size 缓冲区的大小（以字节为单位）。
 * @param data 指向要初始化的数据的指针。
 * @return 返回指向初始化的 ByteBuffer 的指针；如果初始化失败，返回 NULL。
 */
ByteBuffer *bufferInitByteSize(uint64_t size,uint8_t*data);
#endif //NEWDBMS_BYTEBUFFER_H
