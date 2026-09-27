# Release Notes

## v1.0(2026-09-27)

DBMS_C 首个稳定归档版本:教学型 C 语言关系数据库原型,可完整构建、测试与复现。

### 新增

- 纳入此前遗漏的源码与资源,保证全新 clone 可完整构建:
  `Lib/CList.c/h`、`error/DBError.c/h`、`test/Lib/CListTest.c`、`test/error/DBErrorTest.c`、`test/trace/run_trace.bat/.ps1`、`demo/demo.sql`、`demos/trace_select.sql`、`docs/error_handling.md`
- 完整的 13 章配套教程(`docs/tutorial/`,含编译好的 PDF),按模块逐步讲解源码,全部示例配 ctest 验证
- `--trace` 执行链路追踪模式与演示脚本
- `.gitignore` 补全与 `.gitattributes` 行尾规范(源码 LF,`.bat/.ps1` CRLF)
- 开源规范文件:LICENSE(MIT)、CONTRIBUTING、Issue/PR 模板、Windows CI

### 修复

- 恢复被批量处理破坏的中文注释:927 个注释块 + 181 处行尾注释按原始版本还原;多行 doxygen 结构修复
- 保留并整理同批次的代码格式化与防御性加固(CMap / CVector / ByteBuffer 的判空与内存安全检查),业务逻辑零改动
- 清理不可恢复的乱码死代码(`record/RecordPage.c` 中被注释的 DEBUG 打印)
- 命名修正:`Lib/BlockLockManager .c/.h` → `BlockLockManager.c/.h`;`test/tx/TranstionTest.c` → `TransactionTest.c`(CMake 目标与文档同步)
- 移除已被 CList 取代的 `Lib/List.c/h`;解除 `bin/` 构建产物跟踪;清理 LaTeX 中间产物与临时文件

### 测试情况

- 25 个 CMocka 测试套件全部通过
- 验证环境:Windows 11 x64、MinGW-w64 GCC 15.2.0、CMake 3.28.0-rc2
- 复现方式:全新 `git clone` → `cmake -S . -B build` → `cmake --build build -j4` → `ctest`,构建与测试全部通过

### 已知限制

- 仅支持 Windows/MinGW;Linux 移植未完成(部分测试使用 Windows API)
- `CREATE INDEX` 仅登记元数据,哈希索引未接入查询执行路径
- 查询计划为固定组织(`BasicQueryPlanner`),无基于代价的优化
- 启动时不执行自动崩溃恢复,恢复能力为事务回滚撤销级别
- 并发控制为块级 S/X 锁,死锁检测器未接入主流程
- 无网络层、无权限体系、无多客户端会话
