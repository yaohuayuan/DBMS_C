# .github 文件夹

GitHub 平台配置文件,用于项目的持续集成与社区协作规范。

## 内容

- **workflows/ci-cd.yml**:GitHub Actions 工作流,在 Windows(minGW-w64)上执行构建与全部 25 项 ctest 测试;push 到 `main`、提交 PR 或推送 `v*` 标签时触发
- **ISSUE_TEMPLATE/**:Bug 报告与功能建议模板
- **PULL_REQUEST_TEMPLATE.md**:PR 模板,含构建/测试自查清单

## 说明

- 项目当前仅支持 Windows/MinGW,Linux 移植在 Roadmap 中,因此 CI 只保留 Windows job
- 正式版本发布基于 `v*` 标签手动创建 GitHub Release(见 RELEASE_NOTES.md),不做自动发布
