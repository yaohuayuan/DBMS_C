# DBMS_C v1.0 封仓报告

- 封仓日期:2026-09-27
- 归档版本:v1.0(tag)
- 归档提交:`1ed9980 release: prepare v1.0 archive`(140 files, +2804 / -1491)
- 验证环境:Windows 11 x64 / MinGW-w64 GCC 15.2.0 / CMake 3.28.0-rc2

## 1. 修改文件列表(共 140 个)

| 类别 | 数量 | 说明 |
| --- | --- | --- |
| 注释恢复(脚本) | 148 个 .c/.h 中的绝大多数 | 恢复被破坏的中文注释:927 个注释块 + 181 处行尾注释 |
| 注释恢复(人工) | 10 | `File/Page.h`、`metadata/StatManager.h`、`plan/BetterQueryPlanner.h`、`record/TableScan.h`、`query/Scan.h`、`Lib/CMap.c`、`Lib/CVector.c`、`test/buffer/BufferManagerTest.c`、`Lib/ByteBuffer.c`、`record/RecordPage.c` |
| 新纳入版本管理 | 12 | `Lib/CList.c/h`、`error/DBError.c/h`、`test/Lib/CListTest.c`、`test/error/DBErrorTest.c`、`test/trace/run_trace.bat/.ps1`、`demo/demo.sql`、`demos/trace_select.sql`、`docs/error_handling.md`(均为 CMakeLists 已引用或文档已引用的文件) |
| 重命名 | 3 | `Lib/BlockLockManager .c/.h` → `BlockLockManager.c/.h`(去文件名空格);`test/tx/TranstionTest.c` → `TransactionTest.c`(拼写修正,同步更新 CMake 目标名与 test/README.md) |
| 删除/解除跟踪 | 6+1 | `Lib/List.c/h`(已被 CList 取代);`bin/Show/*.tbl`×4、`bin/` 全部构建产物解除跟踪;误入库的根目录旧交付 zip 移出(保留在磁盘) |
| 配置文件 | 2 | `.gitignore` 补全(bin/、cmake 目录、IDE、运行期测试库、zip);新增 `.gitattributes`(源码 LF,`.bat/.ps1` CRLF) |
| 清理临时文件 | 2 | 根目录 `test_report.txt`、`test_trace_input.sql`(无引用的游离临时文件) |

## 2. 修复的问题

1. **中文注释批量破坏(核心问题)**:7 月格式化批次中约 927 个注释块被压扁、断词(如「初始化」→「初始」、「事务」→「事」、doxygen 多行 `@param` 并为一行)。已按 HEAD 原文恢复,同时**保留**了该批次的代码格式化与真实代码改进(CMap/CVector/ByteBuffer 的判空加固等),业务逻辑零改动。
2. **残留 `.git/index.lock`**:已删除(删除前确认无 git 进程),此前会阻塞所有 git 写操作。
3. **clone 后无法构建的隐患**:`error/DBError.c/h`、`Lib/CList.c/h`、`CListTest`、`DBErrorTest` 等被 CMakeLists 引用却从未入库的源码已全部纳入——修复后已在全新 clone 上验证可完整构建。
4. **命名错误**:`BlockLockManager ` 文件名空格、`TranstionTest` 拼写,连同 CMakeLists/README/include 三处引用一并修正。
5. **不可恢复乱码**:`record/RecordPage.c` 中 7 处 GBK 时代遗留的注释掉的 DEBUG printf(已是 U+FFFD 乱码、4 月的 zip 中亦无原文),按死代码删除。
6. **格式化回归**:`Lib/ByteBuffer.c` 3 处多语句被挤在同一行,已拆分为正常多行。
7. **git 卫生**:.gitignore 从 8 行补全至覆盖全部构建产物;bin/ 产物解除跟踪;新增 .gitattributes 消除行尾噪音。

## 3. 未处理问题及原因

| 事项 | 原因 |
| --- | --- |
| `release/` 目录与根目录旧交付 zip(含修复前注释的快照) | 属历史交付物,保留在磁盘但已 gitignore;如需对外重新交付,应从 v1.0 标签重新打包 |
| `Lib/map.c` 与 `Lib/CMap.c` 并存 | 经核实两者均在 CMakeLists 中编译,`map.h` 被元数据层(IndexManager/StatManager/MetadataManager/Schema)引用,是有意并存的两套实现,非冗余 |
| `DeadlockDetector` 未接入主流程 | 既有设计边界;遵循「不新增功能、不改核心设计」未动 |
| 系统启动未调用 `TransactionRecover` | 同上;恢复能力现为「回滚撤销链路」级别,属已知定位 |
| 文件头 `// Created by ...` 横幅 | 格式化批次删除后未回补:无信息量,删除不属于注释破坏 |
| `docs/tutorial/main.pdf` | 既有跟踪的教程编译产物,作为可读文档保留 |
| 行尾统一为 LF | 已通过 .gitattributes 固化;Windows 本地 git 仍会提示 CRLF 转换警告,属正常现象 |

## 4. 构建与测试结果

| 验证项 | 结果 |
| --- | --- |
| 就地重建(全部目标) | ✅ 100% 目标构建成功,无编译错误 |
| 就地 ctest | ✅ 25/25 通过(含改名后的 TransactionTest) |
| 全新 clone → CMake 配置(MinGW Makefiles) | ✅ 配置成功 |
| 全新 clone → 完整构建 | ✅ 100% 目标构建成功(cmocka 由 external/ 提供) |
| 全新 clone → ctest | ✅ 25/25 通过 |
| 乱码/压扁注释全库扫描 | ✅ 0 残留(U+FFFD、`@param.*@param` 签名均清零) |
| git status | ✅ 工作区干净,untracked 为 0 |

## 5. 当前版本状态

- 分支 `main` @ `1ed9980`,领先 `origin/main` 17 个提交(未推送;如需发布请自行 `git push` + `git push --tags`)。
- 标签 `v1.0` 指向封仓提交。
- 教程文档(docs/tutorial,13 章)与架构分析文档(NEWDBMS_ARCH_01~03、PROJECT_ANALYSIS 等)随仓库归档。
- 本仓库为最终归档版本:后续不再新增功能,仅允许修复性改动。
