# DBMS C 库

该仓库包含用于数据库管理系统（DBMS）的一些实用组件的源代码，项目使用 C 语言编写，提供了多种数据结构和函数，包括缓冲区、链表、哈希表、字典树（Trie）和流令牌解析器等，可用于管理数据、处理错误和解析令牌。

## 项目概述

### 主要组件

- **ByteBuffer（字节缓冲区）**：一个灵活的字节缓冲区实现，支持读取、写入、清空和压缩数据。它还支持读取和写入不同类型的数据（字节、短整型、整型、长整型等）。
  
- **Error（错误处理）**：提供了一个基本的错误处理系统，定义了预设的错误代码和自定义错误结构体。
  
- **List（链表）**：实现了一个通用的链表数据结构，用于存储不同类型的数据，支持插入、删除、查找元素和打印链表内容等操作。
  
- **Map（哈希表）**：实现了一个基本的哈希表，支持插入、删除和查询键值对等操作。
  
- **Trie（字典树）**：实现了一个基本的字典树（前缀树），用于高效地存储字符串并支持字符串的查询。

- **StreamTokenizer（流令牌解析器）**：一个解析器，用于解析文本流，识别不同类型的令牌（如单词、整数、分隔符等），并用于进一步处理。

## 文件列表

### `ByteBuffer.c`（缓冲区操作）
- 实现了 `ByteBuffer` 结构体及其相关操作函数，用于操作字节缓冲区。
  
### `Error.c`（错误处理）
- 定义了错误代码和 `Error` 结构体，包括初始化函数。

### `List.c`（链表操作）
- 实现了一个通用的链表，支持动态数据的插入和删除。

### `Map.c`（哈希表操作）
- 实现了一个基本的哈希表，支持键值对存储和操作。

### `Trie.c`（字典树操作）
- 实现了字典树（前缀树）结构，用于高效地存储字符串并进行查找。

### `StreamTokenizer.c`（流令牌解析）
- 定义了流令牌解析器，用于解析字符串流并识别令牌，如单词、整数和分隔符。

## 使用方法

要使用这些组件，您可以在 C 程序中包含相应的头文件：

```c
#include "ByteBuffer.h"
#include "Error.h"
#include "List.h"
#include "Map.h"
#include "Trie.h"
#include "StreamTokenizer.h"
```

### ByteBuffer 使用示例

```c
ByteBuffer *buffer = bufferAllocate(1024);  // 分配一个大小为1024字节的缓冲区
bufferPutInt(buffer, 12345);                // 将一个整数写入缓冲区
ByteBufferData data;
bufferGetInt(buffer, &data);                // 从缓冲区读取整数
printf("数据: %d\n", *data.intData);        // 打印读取的数据
```

### List 使用示例

```c
List *list = ListInit(LIST_TYPE_STRING);    // 创建一个字符串类型的链表
char *data = "Hello, World!";
ListAppend(list, data, strlen(data));      // 向链表中添加数据
ListPrint(list);                           // 打印链表内容
```

### Map 使用示例

```c
map_int_t myMap;
map_init(&myMap);
map_set(&myMap, "key1", 42);               // 向哈希表中设置键值对
map_get(&myMap, "key1");                   // 根据键获取值
```

### Trie 使用示例

```c
Trie *trie = TrieInit();                   // 初始化字典树
TrieInsert(trie, "hello");                 // 插入一个单词
bool found = TrieSearchIn(trie, "hello");  // 查询一个单词
printf("查找结果: %d\n", found);          // 打印查找结果
```

### StreamTokenizer 使用示例

```c
StreamTokenizer tokenizer;
tokenizer_init(&tokenizer, "123 hello, world!");
TokenType token = tokenizer_next(&tokenizer);
printf("令牌类型: %d\n", token);
```

