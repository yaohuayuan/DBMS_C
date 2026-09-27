# DBMS_C 毕业设计代码说明

## 项目简介

本项目是一个基于 C 语言的轻量化教学型 DBMS 原型系统，用于展示 SQL 解析、查询计划、扫描执行、记录管理、元数据管理、事务处理、日志恢复、缓冲区管理和文件存储等数据库底层机制。

系统默认使用 `Show2` 数据库目录。程序启动后会直接进入 SQL 命令行环境，不包含数据库创建、切换或权限管理等工业级管理功能。

## 功能支持

- `CREATE TABLE`
- `INSERT`
- `SELECT`
- `WHERE` 条件查询
- 多表查询
- `UPDATE`
- `DELETE`
- `CREATE VIEW`
- `CREATE INDEX`
- `commit` / `rollback`
- 命令行交互
- CMocka 单元测试

## 目录结构

- `File/`：块、页和数据库文件访问。
- `Log/`：日志写入、日志迭代和恢复支撑。
- `Lib/`：字符串、列表、向量、映射、红黑树等基础工具。
- `buffer/`：缓冲区管理和 LRU 页面替换策略。
- `tx/`：事务、并发控制、恢复管理和缓冲页固定管理。
- `record/`：记录页、表扫描、模式和布局。
- `metadata/`：表、字段、视图、索引和统计信息的元数据管理。
- `parse/`：SQL 词法与语法解析。
- `plan/`：查询计划、更新计划和计划节点。
- `query/`：扫描执行、表达式、谓词和常量。
- `index/`：基础哈希索引实现。
- `error/`：统一错误码和错误信息辅助模块。
- `test/`：CMocka 单元测试。
- `demo/`：毕业设计演示 SQL 脚本。
- `docs/`：项目文档和教程材料。

## 编译方法

在当前 Windows 环境中，直接使用 `cmake --build` 可能触发 Git/MSYS bash 的 Win32 error 5。因此本项目交付时使用 `mingw32-make`，并显式指定 `SHELL=cmd.exe`。

```powershell
cd D:\code\DBMS\NewDBMS\DBMS_C
cmake -S . -B build
mingw32-make -C build -j8 SHELL=cmd.exe
```

## 测试方法

```powershell
ctest --test-dir build --output-on-failure
```

## 运行方法

```powershell
.\bin\NewDBMS.exe
```

程序默认数据库为 `Show2`，启动后进入 SQL 命令行环境：

```text
Database has started successfully. Type 'exit' to close the database.
SQL>
```

## Demo 使用方法

`demo/demo.sql` 中的语句可以逐条复制到 `NewDBMS.exe` 命令行中运行。该脚本默认在 `Show2` 数据库中执行，覆盖建表、插入、查询、条件查询、多表查询、更新、删除、视图、索引和提交等基础功能。

## 项目定位

本项目是本科毕业设计阶段完成的教学型 DBMS 原型，重点在于帮助理解 DBMS 内部机制，不以工业级数据库为目标。代码更关注模块分层、核心数据结构、SQL 执行链路和事务恢复流程的学习价值。

## 后续计划

本项目是本科毕业设计阶段完成的 C 语言轻量化 DBMS 原型。后续作者将在新的仓库 **AI-NATIVE-DBMS-C** 中继续探索数据库系统的重构与扩展工作，重点包括工程结构优化、模块边界整理、错误处理体系、基础容器、DBMS Core 以及 AI 原生语义查询处理相关机制。

AI-NATIVE-DBMS-C 目前仍处于早期建设阶段，主要用于记录后续重构思路、工程骨架和阶段性实现。

项目地址：https://github.com/yaohuayuan/AI-NATIVE-DBMS-C
