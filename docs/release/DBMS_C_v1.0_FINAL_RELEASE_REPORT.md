# DBMS_C v1.0 最终发布报告

- 日期:2026-09-27
- 报告对象:tag `v1.0`(重定位至 `docs: add final release report` 提交)
- 性质:公开 GitHub Release 前的最终整理与审查

## 1. 当前版本状态

| 项 | 状态 |
| --- | --- |
| 分支 | `main`,工作区干净,无未提交修改 |
| 标签 | `v1.0`(annotated,指向最终提交;原标签未推送过远程,重定位无影响),另有历史标签 v0.01 / v0.1 保留 |
| 远程 | `origin = https://github.com/yaohuayuan/DBMS_C.git`,本地领先,**按要求未推送** |
| 大文件 | 已审查全部跟踪文件,无 >1MB 文件;构建产物、LaTeX 中间产物均已忽略/移出 |
| 复现性 | 全新 clone → 构建 → 测试 → 冒烟运行,一次通过(见第 4 节) |

## 2. 完成事项

1. **Release 状态检查**:git status / log / tag / remote 全部核验;无残留 lock、无未提交修改、无误提交大文件。
2. **README 全面重写**:新增项目简介与定位(1 分钟可读)、2 张 Mermaid 架构图(分层架构 + SQL 执行主链路)、诚实的功能列表(明确标注 CREATE INDEX 仅登记元数据)、技术栈、真实示例会话输出(实际运行抓取,非编造)、25 项测试一览、目录结构、文档导航、已知限制、Roadmap、License 章节。徽章:CI / License / C11。
3. **文档整理**:
   - 设计历史归档至 `docs/design/`(PROJECT_ANALYSIS、NEWDBMS_ARCH_01~03、核心数据结构定义手册、模块调用关系图)
   - 发布报告归档至 `docs/release/`(RELEASE_v1.0_REPORT + 本报告)
   - 清理 LaTeX 中间产物磁盘文件(本就未被跟踪);无发现「已废弃需删除」的文档
   - 新增 `RELEASE_NOTES.md`(根目录,v1.0 变更 / 测试情况 / 已知限制)
4. **开源规范文件**(均说明理由):
   - `LICENSE`(MIT):公开仓库无 License = 默认保留所有权利,他人无法合法使用/复现,必须添加
   - `CONTRIBUTING.md`:公开后接收社区 PR 需要统一约定(不改核心设计、测试先行、注释规范)
   - `.github/ISSUE_TEMPLATE/`(bug_report.yml、feature_request.yml):降低无效 Issue 成本,强制提供复现信息与环境
   - `.github/PULL_REQUEST_TEMPLATE.md`:内置构建/测试自查清单
   - 未添加 `CODE_OF_CONDUCT.md`:单人维护的教学项目、外部贡献预期低;社区成长后可补 Contributor Covenant
5. **CI 修正**(`.github/workflows/ci-cd.yml`):删除必然失败的 Linux job(代码含 Windows API)与引用错误路径 `build/DBMS` 的自动 Release job;改为 Windows + MinGW-w64 的 build+test job,push/PR/v* 标签触发
6. **代码审查中的小清理**(非重构):`record/RecordPage.c` 移除 2 处注释掉的 DEBUG 残留与 2 个未使用局部变量(与 v1.0 已有清理同性质,`-Wall` 扫描确认无逻辑问题)

## 3. 文件变化(本次整理)

| 类别 | 文件 |
| --- | --- |
| 重写 | `README.md` |
| 新增 | `RELEASE_NOTES.md`、`LICENSE`、`CONTRIBUTING.md`、`.github/ISSUE_TEMPLATE/bug_report.yml`、`.github/ISSUE_TEMPLATE/feature_request.yml`、`.github/PULL_REQUEST_TEMPLATE.md` |
| 修改 | `.github/workflows/ci-cd.yml`(重写)、`.github/README.md`(更新说明)、`record/RecordPage.c`(死代码清理) |
| 移动 | 7 个设计/发布文档归档至 `docs/design/`、`docs/release/` |

业务逻辑、数据库核心设计、模块边界:零改动。

## 4. 测试与复现记录(模拟陌生用户,全新目录)

环境:Windows 11 x64 / MinGW-w64 GCC 15.2.0 / CMake 3.28.0-rc2 / CMocka 1.1.0(仓库内置)

| 步骤 | 命令 | 结果 |
| --- | --- | --- |
| clone | `git clone <repo> repro_verify` | ✅ 232 个跟踪文件 |
| 配置 | `cmake -G "MinGW Makefiles" -S . -B build` | ✅ Configure/Generating done |
| 构建 | `cmake --build build -j4` | ✅ 100%,0 error |
| 测试 | `cd build && ctest` | ✅ **100% tests passed, 0 failed out of 25** |
| 冒烟 | `echo SQL \| bin\NewDBMS.exe`(建表/插入/查询/退出) | ✅ 结果正确(`\| 7\|`) |

编译警告审查(`-Wall -Wextra` 全量构建):0 error,38 条 warning,全部低危——unused parameter 约 20 条(预留接口扩展点)、有符号/无符号比较 3 条、unused variable/locals 若干(其中 2 处死变量已顺手清理,其余位于未接入主流程的 `BetterQueryPlanner`,不动)。无发现危险内存模式(全库无 U+FFFD/乱码残留,防御性判空已在此前加固)。

## 5. 发布建议

1. 推送:`git push origin main && git push origin --tags`(执行后 CI 会自动跑首轮 Windows 构建)
2. 在 GitHub Releases 页基于 `v1.0` 标签创建 Release:标题 `v1.0`,描述直接粘贴 `RELEASE_NOTES.md` 的 v1.0 小节
3. (可选)打包源码 zip/tar.gz 由 GitHub 自动生成即可,无需手动附可执行文件
4. 仓库设置建议:开启 Issues;分支保护 `main` 要求 PR + CI 绿(单人项目可仅开启状态检查)
5. About 栏填写简介与 `database / c / educational` 话题标签

## 6. 后续维护建议

- **版本节奏**:教学归档版 v1.0 冻结;后续改动走 `fix/`、`docs/` 小步提交,积累到 v1.1 再发 Release
- **新功能方向**(Roadmap 顺序):Linux 移植 → 索引接入查询 → 启动恢复 → 计划优化;每项完成需同步更新 README 已知限制与教程
- **教程同步**:`docs/tutorial/` 各章与 ctest 绑定,改代码务必跑对应章节测试,防止教程与实现脱节
- **重构边界**:`BetterQueryPlanner` 目前未启用,启用前需补齐其独立测试;不要删除(设计历史保留)
- **长期探索**:AI 原生方向已拆分至独立仓库 [AI-NATIVE-DBMS-C](https://github.com/yaohuayuan/AI-NATIVE-DBMS-C),本仓库保持稳定定位
