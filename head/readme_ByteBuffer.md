# ByteBuffer 文档

## 概述

`ByteBuffer` 是一个用于操作字节数据的缓冲区类，类似于 Java 中的 `ByteBuffer` 类。它允许在内存中存储和操作字节数据，并提供了读取和写入不同类型数据的方法。

## 特性

- 动态管理内存，允许设置缓冲区的大小。
- 支持基本的数据操作，包括读取和写入字节、短整数、整数和长整数。
- 提供缓冲区清空、压缩和翻转等操作。

## API

### 数据结构

```c
typedef struct {
    uint64_t size;     // 缓冲区的总大小
    uint8_t* data;     // 数据指针
    uint64_t position; // 当前读写位置
    uint64_t limit;    // 缓冲区的限制（最大可读位置）
} ByteBuffer;
```

### 函数

#### `ByteBuffer* bufferAllocate(uint64_t size)`

分配一个新的 `ByteBuffer` 实例。

- **参数**:
    - `size`: 缓冲区的大小（字节数）。
- **返回值**:
    - 返回一个指向新分配的 `ByteBuffer` 的指针。失败时返回 `NULL`。

#### `void bufferClear(ByteBuffer* buffer)`

清空缓冲区，将所有数据设置为零，并将 `position` 设置为 0，`limit` 设置为缓冲区的大小。

- **参数**:
    - `buffer`: 指向要清空的 `ByteBuffer` 的指针。

#### `void bufferFree(ByteBuffer* buffer)`

释放 `ByteBuffer` 占用的内存。

- **参数**:
    - `buffer`: 指向要释放的 `ByteBuffer` 的指针。

#### `void bufferCompact(ByteBuffer* buffer)`

压缩缓冲区，将当前 `position` 之后的数据移动到缓冲区的开头。

- **参数**:
    - `buffer`: 指向要压缩的 `ByteBuffer` 的指针。

#### `void bufferFlip(ByteBuffer* buffer)`

翻转缓冲区，将 `limit` 设置为当前 `position`，并将 `position` 设置为 0。

- **参数**:
    - `buffer`: 指向要翻转的 `ByteBuffer` 的指针。

#### `uint8_t bufferPut(ByteBuffer* buffer, uint8_t data)`

将一个字节写入缓冲区。

- **参数**:
    - `buffer`: 指向 `ByteBuffer` 的指针。
    - `data`: 要写入的字节。
- **返回值**:
    - 成功时返回 1，失败时返回 0。

#### `uint8_t bufferPutShort(ByteBuffer* buffer, int16_t data)`

将一个 16 位整数写入缓冲区。

- **参数**:
    - `buffer`: 指向 `ByteBuffer` 的指针。
    - `data`: 要写入的 16 位整数。
- **返回值**:
    - 成功时返回 2，失败时返回 0。

#### `uint8_t bufferPutInt(ByteBuffer* buffer, int32_t data)`

将一个 32 位整数写入缓冲区。

- **参数**:
    - `buffer`: 指向 `ByteBuffer` 的指针。
    - `data`: 要写入的 32 位整数。
- **返回值**:
    - 成功时返回 4，失败时返回 0。

#### `uint8_t bufferPutLong(ByteBuffer* buffer, int64_t data)`

将一个 64 位整数写入缓冲区。

- **参数**:
    - `buffer`: 指向 `ByteBuffer` 的指针。
    - `data`: 要写入的 64 位整数。
- **返回值**:
    - 成功时返回 8，失败时返回 0。

#### `uint8_t bufferGet(ByteBuffer* buffer, int8_t* output)`

从缓冲区读取一个字节。

- **参数**:
    - `buffer`: 指向 `ByteBuffer` 的指针。
    - `output`: 指向存储读取结果的内存的指针。
- **返回值**:
    - 成功时返回 1，失败时返回 0。

#### `uint8_t bufferGetShort(ByteBuffer* buffer, int16_t* output)`

从缓冲区读取一个 16 位整数。

- **参数**:
    - `buffer`: 指向 `ByteBuffer` 的指针。
    - `output`: 指向存储读取结果的内存的指针。
- **返回值**:
    - 成功时返回 2，失败时返回 0。

#### `uint8_t bufferGetInt(ByteBuffer* buffer, int32_t* output)`

从缓冲区读取一个 32 位整数。

- **参数**:
    - `buffer`: 指向 `ByteBuffer` 的指针。
    - `output`: 指向存储读取结果的内存的指针。
- **返回值**:
    - 成功时返回 4，失败时返回 0。

#### `uint8_t bufferGetLong(ByteBuffer* buffer, int64_t* output)`

从缓冲区读取一个 64 位整数。

- **参数**:
    - `buffer`: 指向 `ByteBuffer` 的指针。
    - `output`: 指向存储读取结果的内存的指针。
- **返回值**:
    - 成功时返回 8，失败时返回 0。

#### `uint64_t bufferGetBytes(ByteBuffer* buffer, void* output, uint64_t size)`

从缓冲区读取指定数量的字节。

- **参数**:
    - `buffer`: 指向 `ByteBuffer` 的指针。
    - `output`: 指向存储读取结果的内存的指针。
    - `size`: 要读取的字节数。
- **返回值**:
    - 成功时返回读取的字节数，失败时返回 0。

## 使用示例

```c
#include <stdio.h>
#include "ByteBuffer.h"

int main() {
    ByteBuffer* buffer = bufferAllocate(64);
    if (buffer == NULL) {
        printf("分配缓冲区失败。\n");
        return 1;
    }

    // 写入和读取一个字节
    bufferPut(buffer, 0xAB);
    bufferFlip(buffer);
    uint8_t byte;
    bufferGet(buffer, &byte);
    printf("读取的字节: 0x%X\n", byte);

    // 写入和读取一个短整数
    bufferClear(buffer);
    bufferPutShort(buffer, 0x1234);
    bufferFlip(buffer);
    int16_t shortValue;
    bufferGetShort(buffer, &shortValue);
    printf("读取的短整数: 0x%X\n", shortValue);

    // 写入和读取一个整数
    bufferClear(buffer);
    bufferPutInt(buffer, 0x12345678);
    bufferFlip(buffer);
    int32_t intValue;
    bufferGetInt(buffer, &intValue);
    printf("读取的整数: 0x%X\n", intValue);

    // 写入和读取一个长整数
    bufferClear(buffer);
    bufferPutLong(buffer, 0x1234567890ABCDEF);
    bufferFlip(buffer);
    int64_t longValue;
    bufferGetLong(buffer, &longValue);
    printf("读取的长整数: 0x%lX\n", longValue);

    bufferFree(buffer);
    return 0;
}
```

## 构建

在编译时，确保包含 `ByteBuffer.c` 和 `ByteBuffer.h` 文件，例如：

```sh
gcc -o test test.c ByteBuffer.c
```

