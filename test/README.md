# test 文件夹

该文件夹包含数据库系统的测试用例，用于验证各个模块的功能和性能。

## 内容

- **File/**: 文件管理相关的测试用例
  - **BlockIDTest.c**: 块标识符的测试
  - **FileManagerTest.c**: 文件管理器的测试
  - **FileTest.c**: 文件操作的测试
  - **PageTest.c**: 页操作的测试
- **Lib/**: 基础库相关的测试用例
  - **ByteBufferTest.c**: 字节缓冲区的测试
  - **CMapTest.c**: 映射数据结构的测试
  - **CStringTest.c**: 字符串处理的测试
  - **CVectorTest.c**: 向量数据结构的测试
  - **RBTTest.c**: 红黑树数据结构的测试
  - **mapTest.c**: 映射数据结构的另一种测试
- **Log/**: 日志管理相关的测试用例
  - **LogTest.c**: 日志管理器的测试
- **buffer/**: 缓冲区管理相关的测试用例
  - **LRU/**: LRU页面替换算法的测试
    - **LRUPolicyTest.c**: LRU策略的测试
  - **BufferManagerTest.c**: 缓冲区管理器的测试
  - **BufferTest.c**: 缓冲区操作的测试
- **record/**: 记录管理相关的测试用例
  - **RecordTest.c**: 记录操作的测试
- **tx/**: 事务管理相关的测试用例
  - **LockTest.c**: 锁机制的测试
  - **TranstionTest.c**: 事务处理的测试
- **Test.c**: 主测试入口
- **test_bench_tps.c**: TPS性能基准测试

## 用途

- 验证各个模块的功能正确性
- 测试系统在各种情况下的行为
- 检测和修复系统中的bug
- 性能测试和基准测试
- 确保系统的稳定性和可靠性