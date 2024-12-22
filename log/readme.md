# DBMS - 日志管理系统

## 项目简介

本项目实现了一个简单的数据库管理系统（DBMS）中的日志管理系统。日志管理系统主要用于记录数据库的变更操作，通过日志的方式提供事务的回滚、恢复等功能。项目包括日志的写入、读取、迭代以及日志的持久化操作。

## 主要功能

1. **日志迭代器（LogIterator）**
    - 提供遍历日志文件的功能，通过 `LogIterator` 实现逐条读取日志。
    - 支持移动到指定日志块，判断是否有下一个日志条目。

2. **日志管理器（LogManager）**
    - 管理日志文件的操作，包括日志条目的追加和刷新。
    - 提供日志文件的持久化机制，确保日志数据被及时保存。

3. **日志条目追加**
    - 支持向日志文件追加新的日志条目，并返回最新的日志序列号（LSN）。

4. **日志持久化**
    - 提供日志刷新操作，将内存中的日志数据写入磁盘。
    - 支持指定 LSN 的日志条目刷新。


## 编译与运行

### 1. 安装依赖

项目主要使用 C 语言进行开发，确保系统中安装了 `gcc` 编译器以及 `make` 工具。

### 2. 编译项目

在项目根目录下运行以下命令进行编译：

```bash
make
```

### 3. 运行项目

编译完成后，生成的可执行文件位于 `bin/` 目录下。可以通过以下命令运行项目中的测试代码：

```bash
./bin/log_iterator_test   # 运行日志迭代器测试
./bin/log_manager_test    # 运行日志管理器测试
```

### 4. 清理

若需要清理编译生成的文件，可以运行：

```bash
make clean
```

## 使用说明

### LogIterator

`LogIterator` 是用于遍历日志文件的核心模块。通过该模块，你可以逐条读取日志条目。

#### 初始化

```c
LogIterator* LogIteratorInit(FileManager *fm, BlockID blockId);
```

- **fm**：文件管理器实例。
- **blockId**：日志块的起始 ID。

#### 移动到指定日志块

```c
void LogIteratorMoveToBlock(LogIterator* logIterator);
```

#### 检查是否有更多日志条目

```c
bool LogIteratorHasNext(LogIterator *logIterator);
```

#### 获取下一个日志条目

```c
ByteBufferData* LogIteratorNext(LogIterator *logIterator);
```

### LogManager

`LogManager` 负责日志的持久化管理，支持将日志条目追加到文件，并在磁盘上保存数据。

#### 初始化

```c
LogManager* LogManagerInit(FileManager *fileManager, char *logfile);
```

#### 追加新日志块

```c
BlockID LogManagerAppendNewBlock(LogManager* logManager);
```

#### 刷新日志到磁盘

```c
void LogManagerFlush(LogManager *logManager);
```

#### 向日志文件追加数据

```c
int LogManagerAppend(LogManager *logManager, uint8_t *data, uint8_t *type, int size);
```

### 日志文件格式

日志文件包含多个日志块，每个日志块由若干条日志记录组成。每条日志记录包括以下信息：

- 数据类型
- 数据内容
- 日志序列号（LSN）

### 示例

```c
FileManager *fm = FileManagerInit("logfile.dat");
LogManager *logManager = LogManagerInit(fm, "logfile.dat");

// 向日志中追加数据
uint8_t data[] = {0x01, 0x02, 0x03};
uint8_t type[] = {0x01};
int size = sizeof(data);
int lsn = LogManagerAppend(logManager, data, type, size);

// 刷新日志
LogManagerFlush(logManager);
```


