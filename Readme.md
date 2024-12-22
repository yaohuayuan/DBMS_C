# DBMS设计C语言版
## 项目介绍
这里是C语言版DBMS,是仿照开源Java版编写的一个DBMS.  
本人小白编写,可能会有一些BUG,欢迎大家指正.  
有帮助的话可以Star一下哦~  
有其他想编写的可以私信我哦
## 项目难点实现
此项目为C语言改写Java版本，所以底层公共库的编写，也有多态实现。

公共库实现都放在head中，其中map是为[!此地址](https://github.com/rxi/map?tab=readme-ov-file)的map文件.

多态是采用枚举类型+函数指针+共用体变量.


## 文件结构
### file(文件管理)
1. BlockID
2. Page
3. FileManager
### head(一些数据结构实现)
1. ByteBuffer
2. map
3. List
4. StreamTokenizer
5. Trie
### log
1. LogIterator
2. LogManager
### buffer
1. Buffer
2. BufferManager
### record
1. Layout
2. RID
3. Schema
4. RecordPage
5. TableScan
### tx
1. BufferList
2. Transaction 
#### concurrency
1. LockTable
2. ConcurrencyManager
#### recovery
1. LogRecord
2. RecoveryManager
### query
1. Constant
2. Expression
3. Predicate
4. ProductScan
5. ProjectScan
6. Scan
7. SelectScan
8. Term
9. UpdateScan
### metadata
1. IndexInfo
2. IndexManager
3. MetadataManager
4. StatManager
5. Stat
6. TableManager
7. ViewManager
### parser
1. Lexer
2. Parser
### plan
1. BasicQueryPlanner
2. BasicUpdatePlanner
3. Plan
4. Planner
5. ProductPlan
6. ProjectPlan
7. SelectPlan
8. TablePlan
### server
SimpleDB
### test
test
### Cmake
CmakeLists.txt
## 运行
### 环境：
须在Windows上继续运行
### 运行
在Clion上直接运行或修改Cmake文件进行运行
