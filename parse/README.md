# parse 文件夹

该文件夹包含SQL解析相关的实现，负责将SQL语句解析为内部数据结构。

## 内容

- **CreateIndexData.c/h**: 创建索引语句的数据结构和处理。
- **CreateTableData.c/h**: 创建表语句的数据结构和处理。
- **CreateViewData.c/h**: 创建视图语句的数据结构和处理。
- **DeleteData.c/h**: 删除语句的数据结构和处理。
- **InsertData.c/h**: 插入语句的数据结构和处理。
- **Lexer.c/h**: 词法分析器，将SQL语句分解为标记。
- **ModifyData.c/h**: 修改语句的数据结构和处理。
- **Parser.c/h**: 语法分析器，将标记序列解析为语法树。
- **PredParser.c/h**: 谓词解析器，负责解析WHERE子句中的谓词。
- **QueryData.c/h**: 查询语句的数据结构和处理。

## 用途

- 实现SQL语句的词法分析
- 实现SQL语句的语法分析
- 将SQL语句转换为内部数据结构
- 支持各种SQL语句类型（查询、插入、更新、删除、创建等）
- 处理SQL语句中的谓词和表达式