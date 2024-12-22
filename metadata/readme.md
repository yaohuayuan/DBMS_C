# Database Management System (DBMS) - MetadataManager

## 项目概述

该项目是一个数据库管理系统的实现，主要涉及多个管理器的设计与实现，包括表管理器 (`TableManager`)、视图管理器 (`ViewManager`)、元数据管理器 (`MetadataManager`)、索引管理器 (`IndexManager`)、统计管理器 (`StartManager`) 以及与之相关的功能。

本项目主要用 C 语言开发，旨在为数据库系统提供基本的表、视图、索引、统计数据等管理功能。

---

## 文件结构

以下是项目中的主要文件和它们的功能描述：

1. **`TableManager.h`**
    - 定义了表管理器 (`TableManager`) 结构体，用于管理数据库中的表及其相关操作。
    - 主要功能：
        - 创建新表 (`TableManagerCreateTable`)。
        - 获取表的布局信息 (`TableManagerGetLayout`)。
        - 初始化表管理器 (`TableManagerInit`)。

2. **`ViewManager.h`**
    - 定义了视图管理器 (`ViewManager`) 结构体，用于管理数据库中的视图。
    - 主要功能：
        - 创建新视图 (`ViewManagerCreateView`)。
        - 获取视图定义 (`ViewManagerGetViewDef`)。
        - 初始化视图管理器 (`ViewManagerInit`)。

3. **`MetadataManager.h`**
    - 定义了元数据管理器 (`MetadataMgr`) 结构体，用于管理数据库的元数据（表、视图、索引等）。
    - 主要功能：
        - 创建新表 (`MetadataMgrCreateTable`)。
        - 创建新视图 (`MetadataMgrCreateView`)。
        - 创建新索引 (`MetadataMgrCreateIndex`)。
        - 获取表的统计信息 (`MetadataMgrGetStatInfo`)。
        - 获取索引信息 (`MetadataMgrGetIndexInfo`)。

4. **`IndexManager.h`**
    - 定义了索引管理器 (`IndexMgr`) 结构体，用于管理数据库中的索引。
    - 主要功能：
        - 创建索引 (`IndexMgrCreateIndex`)。
        - 获取表的索引信息 (`IndexMgrGetIndexInfo`)。
        - 初始化索引管理器 (`IndexMgrInit`)。

5. **`StatInfo.h`**
    - 定义了表的统计信息结构体 (`StatInfo`)，包含表的块数量、记录数量等统计信息。
    - 主要功能：
        - 获取表的块数量 (`StatInfoBlocksAccessed`)。
        - 获取表的记录数量 (`StatInfoRecordsOutput`)。
        - 获取字段的不同值数量 (`StatInfoDistinctValues`)。
        - 初始化统计信息 (`StatInfoInit`)。

6. **`StartManager.h`**
    - 定义了统计管理器 (`StartManager`) 结构体，用于管理数据库中的统计数据。
    - 主要功能：
        - 获取表的统计信息 (`StartManagerGetStatInfo`)。
        - 刷新统计信息 (`StartManagerRefreshStatistics`)。
        - 计算并更新表的统计信息 (`StartManagerCalcTableStats`)。
        - 初始化统计管理器 (`StartManagerInit`)。

---

## 编译与使用

### 依赖库

该项目依赖以下库：

- `map`：用于实现哈希表等数据结构。

### 编译指令

项目使用 C 语言编写，确保使用支持 C99 或更高版本标准的编译器进行编译。

```bash
gcc -o dbms main.c TableManager.c ViewManager.c MetadataManager.c IndexManager.c StatInfo.c StartManager.c -lm
```

### 运行示例

在项目目录中，使用以下命令进行编译和运行：

```bash
make  # 使用 makefile 编译项目
./dbms  # 运行编译后的程序
```

具体的数据库管理操作可以通过调用相关管理器的函数来实现。

---

## 功能说明

1. **表管理** (`TableManager`)
    - 支持创建和管理数据库中的表。提供获取表布局信息、初始化表管理器等功能。

2. **视图管理** (`ViewManager`)
    - 支持创建和管理数据库中的视图，允许用户获取视图的定义信息。

3. **元数据管理** (`MetadataManager`)
    - 管理数据库的元数据，包括表的创建、视图和索引的管理。支持获取表的布局信息、统计信息以及索引信息。

4. **索引管理** (`IndexManager`)
    - 管理数据库中的索引，包括索引的创建和获取表的索引信息。

5. **统计信息管理** (`StatInfo` & `StartManager`)
    - 提供表的统计信息，如块数、记录数和字段的不同值数。`StartManager` 用于更新和管理这些统计信息。

