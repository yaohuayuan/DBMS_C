//
// Created by yaohuayuan on 2024/9/18.
//

#ifndef DBMS_C_BYTEBUFFER_H
#define DBMS_C_BYTEBUFFER_H

#include <stdint.h>
#include <malloc.h>
#include <string.h>
#define BYTEBUFFER_STATUS int

typedef enum {
    BYTEBUFFER_INIT = 0,
    BYTEBUFFER_INT,
    BYTEBUFFER_SHORT,
    BYTEBUFFER_LONG,
    BYTEBUFFER_STRING,
    BYTEBUFFER_BYTE,
    BYTEBUFFER_BYTES,
} BYTEBUFFER_STATUS_CODE;

// ByteBuffer 结构体定义
typedef struct
{
    uint64_t size;      // 缓冲区总大小（字节数）
    uint8_t* data;      // 指向缓冲区数据的指针
    uint64_t position;  // 当前读写位置
    uint64_t limit;     // 缓冲区的限制（即读写的最大位置）
    uint8_t *type;
} ByteBuffer;
typedef struct{
    int length;
    char *data;
}StringData;
typedef struct{
    int length;
    uint8_t *data;
    uint8_t *type;
}BytesData;
typedef struct {
    BYTEBUFFER_STATUS StatusCode;
    int * intData;
    short *shortData;
    long long* longData;
    StringData *stringData;
    uint8_t *byteData;
    BytesData *bytesData;
}ByteBufferData;
ByteBufferData* ByteBufferDataInit();
/**
 * 分配一个新的 ByteBuffer 实例，并为其分配指定大小的内存。
 *
 * @param size 缓冲区的大小（以字节为单位）。
 * @return 指向分配的 ByteBuffer 的指针；如果分配失败，返回 NULL。
 */
ByteBuffer* bufferAllocate(uint64_t size);

/**
 * 清除缓冲区，将其所有字节设置为零，并将 position 设为 0，将 limit 设为缓冲区的大小。
 *
 * @param buffer 指向要清除的 ByteBuffer 的指针。
 */
void bufferClear(ByteBuffer* buffer);

/**
 * 释放 ByteBuffer 实例及其数据内存。
 *
 * @param buffer 指向要释放的 ByteBuffer 的指针。
 */
void bufferFree(ByteBuffer* buffer);

/**
 * 压缩缓冲区，将当前位置之后的数据移动到缓冲区的开始位置，并更新 position 和 limit。
 *
 * @param buffer 指向要压缩的 ByteBuffer 的指针。
 */
void bufferCompact(ByteBuffer* buffer);

/**
 * 翻转缓冲区，将 limit 设为当前 position，并将 position 设为 0。
 *
 * @param buffer 指向要翻转的 ByteBuffer 的指针。
 */
void bufferFlip(ByteBuffer* buffer);

/**
 * 将一个字节数据写入缓冲区当前位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param data 要写入的数据（一个字节）。
 * @return 如果写入成功，返回 1；否则返回 0。
 */
uint8_t bufferPut(ByteBuffer* buffer, uint8_t data);

/**
 * 将一个 16 位的整数数据写入缓冲区当前位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param data 要写入的数据（16 位整数）。
 * @return 如果写入成功，返回 2；否则返回 0。
 */
uint8_t bufferPutShort(ByteBuffer* buffer, int16_t data);

/**
 * 将一个 32 位的整数数据写入缓冲区当前位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param data 要写入的数据（32 位整数）。
 * @return 如果写入成功，返回 4；否则返回 0。
 */
uint8_t bufferPutInt(ByteBuffer* buffer, int32_t data);

/**
 * 将一个 64 位的整数数据写入缓冲区当前位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param data 要写入的数据（64 位整数）。
 * @return 如果写入成功，返回 8；否则返回 0。
 */
uint8_t bufferPutLong(ByteBuffer* buffer, int64_t data);

/**
 * 从缓冲区当前位置读取一个字节数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param output 指向用于存储读取数据的 int8_t 变量的指针。
 * @return 如果读取成功，返回 1；否则返回 0。
 */

uint8_t bufferGet(ByteBuffer* buffer, ByteBufferData *data);

/**
 * 从缓冲区当前位置读取一个 16 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param output 指向用于存储读取数据的 int16_t 变量的指针。
 * @return 如果读取成功，返回 2；否则返回 0。
 */
uint8_t bufferGetShort(ByteBuffer* buffer, ByteBufferData *data);

/**
 * 从缓冲区当前位置读取一个 32 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param output 指向用于存储读取数据的 int32_t 变量的指针。
 * @return 如果读取成功，返回 4；否则返回 0。
 */
uint8_t bufferGetInt(ByteBuffer* buffer, ByteBufferData *data);

/**
 * 从缓冲区当前位置读取一个 64 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param output 指向用于存储读取数据的 int64_t 变量的指针。
 * @return 如果读取成功，返回 8；否则返回 0。
 */
uint8_t bufferGetLong(ByteBuffer* buffer, ByteBufferData *data);

/**
 * 从缓冲区当前位置读取指定数量的字节数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param output 指向用于存储读取数据的内存区域的指针。
 * @param size 要读取的数据字节数。
 * @return 成功读取的字节数；如果读取失败，返回 0。
 */
uint64_t bufferGetBytes(ByteBuffer* buffer, ByteBufferData *data, int size);
/**
 * 将一个字节数据写入缓冲区的指定位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 写入字节的位置。
 * @param data 要写入的数据（一个字节）。
 * @return 如果写入成功，返回 1；否则返回 0。
 */
uint8_t bufferPutPosition(ByteBuffer* buffer, uint64_t position, uint8_t data);

/**
 * 将一个 16 位的整数数据写入缓冲区的指定位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 写入数据的位置。
 * @param data 要写入的数据（16 位整数）。
 * @return 如果写入成功，返回 2；否则返回 0。
 */
uint8_t bufferPutShortPosition(ByteBuffer* buffer, uint64_t position, int16_t data);

/**
 * 将一个 32 位的整数数据写入缓冲区的指定位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 写入数据的位置。
 * @param data 要写入的数据（32 位整数）。
 * @return 如果写入成功，返回 4；否则返回 0。
 */
uint8_t bufferPutIntPosition(ByteBuffer* buffer, uint64_t position, int32_t data);

/**
 * 将一个 64 位的整数数据写入缓冲区的指定位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 写入数据的位置。
 * @param data 要写入的数据（64 位整数）。
 * @return 如果写入成功，返回 8；否则返回 0。
 */
uint8_t bufferPutLongPosition(ByteBuffer* buffer, uint64_t position, int64_t data);

/**
 * 从缓冲区的指定位置读取一个字节数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 读取字节的位置。
 * @param output 指向用于存储读取数据的 int8_t 变量的指针。
 * @return 如果读取成功，返回 1；否则返回 0。
 */
uint8_t bufferGetPosition(ByteBuffer* buffer, uint64_t position, ByteBufferData *output);

/**
 * 从缓冲区的指定位置读取一个 16 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 读取数据的位置。
 * @param output 指向用于存储读取数据的 int16_t 变量的指针。
 * @return 如果读取成功，返回 2；否则返回 0。
 */
uint8_t bufferGetShortPosition(ByteBuffer* buffer, uint64_t position, ByteBufferData *output);

/**
 * 从缓冲区的指定位置读取一个 32 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 读取数据的位置。
 * @param output 指向用于存储读取数据的 int32_t 变量的指针。
 * @return 如果读取成功，返回 4；否则返回 0。
 */
uint8_t bufferGetIntPosition(ByteBuffer* buffer, uint64_t position, ByteBufferData* output);

/**
 * 从缓冲区的指定位置读取一个 64 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 读取数据的位置。
 * @param output 指向用于存储读取数据的 int64_t 变量的指针。
 * @return 如果读取成功，返回 8；否则返回 0。
 */
uint8_t bufferGetLongPosition(ByteBuffer* buffer, uint64_t position, ByteBufferData* output);

/**
 * 从缓冲区的指定位置读取指定数量的字节数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 读取数据的起始位置。
 * @param output 指向用于存储读取数据的内存区域的指针。
 * @param size 要读取的数据字节数。
 * @return 成功读取的字节数；如果读取失败，返回 0。
 */
uint64_t bufferGetBytesPosition(ByteBuffer* buffer, uint64_t position, ByteBufferData *data, int size);

/**
 * 改变缓冲区的position
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param newPosition 读取数据的起始位置。。
 * @return 返回成功改变的position。
 */
uint64_t bufferPosition(ByteBuffer* buffer, uint64_t newPosition);

uint64_t bufferPutBytesPosition(ByteBuffer* buffer, uint64_t position, uint8_t* data,uint8_t *type, uint64_t size);

uint64_t bufferPutBytes(ByteBuffer* buffer, uint8_t* data, uint8_t *type,uint64_t size);


#endif //DBMS_C_BYTEBUFFER_H
