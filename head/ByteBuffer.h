//
// Created by yaohuayuan on 2024/9/18.
//

#ifndef DBMS_C_BYTEBUFFER_H
#define DBMS_C_BYTEBUFFER_H

#include <stdint.h>
#include <malloc.h>
#include <string.h>

/**
 * @brief 定义 ByteBuffer 的状态码枚举，用于标识当前缓冲区的操作类型。
 */
typedef enum {
    BYTEBUFFER_INIT = 0,      ///< 初始化状态
    BYTEBUFFER_INT,           ///< 整数类型
    BYTEBUFFER_SHORT,         ///< 短整数类型
    BYTEBUFFER_LONG,          ///< 长整数类型
    BYTEBUFFER_STRING,        ///< 字符串类型
    BYTEBUFFER_BYTE,          ///< 单字节类型
    BYTEBUFFER_BYTES          ///< 字节数组类型
} BYTEBUFFER_STATUS_CODE;

/**
 * @brief ByteBuffer 结构体定义，用于管理内存中的字节数据。
 */
typedef struct {
    uint64_t size;       ///< 缓冲区总大小（字节数）
    uint8_t* data;       ///< 指向缓冲区数据的指针
    uint64_t position;   ///< 当前读写位置
    uint64_t limit;      ///< 缓冲区的限制（即读写的最大位置）
    uint8_t *type;       ///< 数据类型信息（可选）
} ByteBuffer;

/**
 * @brief ByteBufferData 结构体定义，用于封装从 ByteBuffer 中读取的数据。
 */
typedef struct {
    int length;          ///< 数据长度
    char *data;          ///< 数据内容（字符串形式）
} StringData;

typedef struct {
    int length;          ///< 数据长度
    uint8_t *data;       ///< 数据内容（字节数组形式）
    uint8_t *type;       ///< 数据类型信息（可选）
} BytesData;

typedef struct {
    BYTEBUFFER_STATUS_CODE StatusCode; ///< 数据的状态码
    int *intData;                      ///< 整数数据指针
    short *shortData;                  ///< 短整数数据指针
    long long* longData;               ///< 长整数数据指针
    StringData *stringData;            ///< 字符串数据指针
    uint8_t *byteData;                 ///< 单字节数据指针
    BytesData *bytesData;              ///< 字节数组数据指针
} ByteBufferData;

/**
 * @brief 分配一个新的 ByteBufferData 实例。
 *
 * @return 返回初始化后的 ByteBufferData 指针。
 */
ByteBufferData* ByteBufferDataInit();

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
 * @return 如果写入成功，返回 1；否则返回 0。
 */
uint8_t bufferPut(ByteBuffer* buffer, uint8_t data);

/**
 * @brief 将一个 16 位的整数数据写入缓冲区当前位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param data 要写入的数据（16 位整数）。
 * @return 如果写入成功，返回 2；否则返回 0。
 */
uint8_t bufferPutShort(ByteBuffer* buffer, int16_t data);

/**
 * @brief 将一个 32 位的整数数据写入缓冲区当前位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param data 要写入的数据（32 位整数）。
 * @return 如果写入成功，返回 4；否则返回 0。
 */
uint8_t bufferPutInt(ByteBuffer* buffer, int32_t data);

/**
 * @brief 将一个 64 位的整数数据写入缓冲区当前位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param data 要写入的数据（64 位整数）。
 * @return 如果写入成功，返回 8；否则返回 0。
 */
uint8_t bufferPutLong(ByteBuffer* buffer, int64_t data);

/**
 * @brief 从缓冲区当前位置读取一个字节数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param output 指向用于存储读取数据的 ByteBufferData 指针。
 * @return 如果读取成功，返回 1；否则返回 0。
 */
uint8_t bufferGet(ByteBuffer* buffer, ByteBufferData *output);

/**
 * @brief 从缓冲区当前位置读取一个 16 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param output 指向用于存储读取数据的 ByteBufferData 指针。
 * @return 如果读取成功，返回 2；否则返回 0。
 */
uint8_t bufferGetShort(ByteBuffer* buffer, ByteBufferData *output);

/**
 * @brief 从缓冲区当前位置读取一个 32 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param output 指向用于存储读取数据的 ByteBufferData 指针。
 * @return 如果读取成功，返回 4；否则返回 0。
 */
uint8_t bufferGetInt(ByteBuffer* buffer, ByteBufferData *output);

/**
 * @brief 从缓冲区当前位置读取一个 64 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param output 指向用于存储读取数据的 ByteBufferData 指针。
 * @return 如果读取成功，返回 8；否则返回 0。
 */
uint8_t bufferGetLong(ByteBuffer* buffer, ByteBufferData *output);

/**
 * @brief 从缓冲区当前位置读取指定数量的字节数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param output 指向用于存储读取数据的 ByteBufferData 指针。
 * @param size 要读取的数据字节数。
 * @return 成功读取的字节数；如果读取失败，返回 0。
 */
uint64_t bufferGetBytes(ByteBuffer* buffer, ByteBufferData *output, int size);

/**
 * @brief 将一个字节数据写入缓冲区的指定位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 写入字节的位置。
 * @param data 要写入的数据（一个字节）。
 * @return 如果写入成功，返回 1；否则返回 0。
 */
uint8_t bufferPutPosition(ByteBuffer* buffer, uint64_t position, uint8_t data);

/**
 * @brief 将一个 16 位的整数数据写入缓冲区的指定位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 写入数据的位置。
 * @param data 要写入的数据（16 位整数）。
 * @return 如果写入成功，返回 2；否则返回 0。
 */
uint8_t bufferPutShortPosition(ByteBuffer* buffer, uint64_t position, int16_t data);

/**
 * @brief 将一个 32 位的整数数据写入缓冲区的指定位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 写入数据的位置。
 * @param data 要写入的数据（32 位整数）。
 * @return 如果写入成功，返回 4；否则返回 0。
 */
uint8_t bufferPutIntPosition(ByteBuffer* buffer, uint64_t position, int32_t data);

/**
 * @brief 将一个 64 位的整数数据写入缓冲区的指定位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 写入数据的位置。
 * @param data 要写入的数据（64 位整数）。
 * @return 如果写入成功，返回 8；否则返回 0。
 */
uint8_t bufferPutLongPosition(ByteBuffer* buffer, uint64_t position, int64_t data);

/**
 * @brief 从缓冲区的指定位置读取一个字节数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 读取字节的位置。
 * @param output 指向用于存储读取数据的 ByteBufferData 指针。
 * @return 如果读取成功，返回 1；否则返回 0。
 */
uint8_t bufferGetPosition(ByteBuffer* buffer, uint64_t position, ByteBufferData *output);

/**
 * @brief 从缓冲区的指定位置读取一个 16 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 读取数据的位置。
 * @param output 指向用于存储读取数据的 ByteBufferData 指针。
 * @return 如果读取成功，返回 2；否则返回 0。
 */
uint8_t bufferGetShortPosition(ByteBuffer* buffer, uint64_t position, ByteBufferData *output);

/**
 * @brief 从缓冲区的指定位置读取一个 32 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 读取数据的位置。
 * @param output 指向用于存储读取数据的 ByteBufferData 指针。
 * @return 如果读取成功，返回 4；否则返回 0。
 */
uint8_t bufferGetIntPosition(ByteBuffer* buffer, uint64_t position, ByteBufferData *output);

/**
 * @brief 从缓冲区的指定位置读取一个 64 位的整数数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 读取数据的位置。
 * @param output 指向用于存储读取数据的 ByteBufferData 指针。
 * @return 如果读取成功，返回 8；否则返回 0。
 */
uint8_t bufferGetLongPosition(ByteBuffer* buffer, uint64_t position, ByteBufferData *output);

/**
 * @brief 从缓冲区的指定位置读取指定数量的字节数据。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param position 读取数据的起始位置。
 * @param output 指向用于存储读取数据的 ByteBufferData 指针。
 * @param size 要读取的数据字节数。
 * @return 成功读取的字节数；如果读取失败，返回 0。
 */
uint64_t bufferGetBytesPosition(ByteBuffer* buffer, uint64_t position, ByteBufferData *output, int size);

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
 * @param type 数据类型信息（可选）。
 * @param size 数据长度（字节数）。
 * @return 成功写入的字节数；如果写入失败，返回 0。
 */
uint64_t bufferPutBytesPosition(ByteBuffer* buffer, uint64_t position, uint8_t* data, uint8_t *type, uint64_t size);

/**
 * @brief 将一段字节数组写入缓冲区当前位置。
 *
 * @param buffer 指向 ByteBuffer 的指针。
 * @param data 要写入的数据（字节数组）。
 * @param type 数据类型信息（可选）。
 * @param size 数据长度（字节数）。
 * @return 成功写入的字节数；如果写入失败，返回 0。
 */
uint64_t bufferPutBytes(ByteBuffer* buffer, uint8_t* data, uint8_t *type, uint64_t size);

#endif //DBMS_C_BYTEBUFFER_H