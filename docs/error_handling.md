# Error Handling

本项目逐步引入 `DBStatus + DBError` 作为统一错误处理基础设施。目标是让底层模块可以返回稳定的状态码，同时把便于调试的信息保存在调用方提供的错误对象中。

## 基本模型

`DBStatus` 表示函数调用是否成功。`DB_OK` 表示成功，其他状态码表示不同类别的失败，例如参数错误、文件读写失败、语法错误、锁冲突、日志写入失败或内部错误。

`DBError` 记录一次失败的上下文，包括模块名、源文件、行号和错误消息。`DB_SET_ERROR`、`DB_SET_ERRORF`、`DB_RETURN_ERROR` 会自动记录 `__FILE__` 和 `__LINE__`。

推荐新接口返回 `DBStatus`，实际结果通过 `out` 参数返回：

```c
DBStatus ModuleTryDoWork(Input *input, Result **out, DBError *err) {
    if (!input || !out) {
        DB_RETURN_ERROR(err, DB_ERR_INVALID_ARGUMENT, "Module", "input or out is NULL");
    }

    *out = NULL;
    return DB_OK;
}
```

调用方可以在栈上创建 `DBError`：

```c
DBError err;
DBErrorInit(&err);

Result *result = NULL;
DBStatus status = ModuleTryDoWork(input, &result, &err);
if (!DBStatusIsOk(status)) {
    DBErrorPrint(&err, stderr);
}
```

## 运行 DBErrorTest

`DBErrorTest` 已注册到 CMake/CMocka 测试集中。当前 Windows 环境下不要使用 `cmake --build build`，应使用项目已验证的固定构建命令：

```powershell
cd D:\code\DBMS\NewDBMS\DBMS_C
cmake -S . -B build
mingw32-make -C build -j8 SHELL=cmd.exe
ctest --test-dir build --output-on-failure
```

如只想运行 DBError 测试，可以在完成配置和编译后执行：

```powershell
ctest --test-dir build -R DBErrorTest --output-on-failure
```

## DBErrorTest 覆盖内容

`test/error/DBErrorTest.c` 覆盖以下基础行为：

1. `DBErrorInit` 将错误对象初始化为 `DB_OK`，并清空 message。
2. `DBStatusIsOk` 能正确区分 `DB_OK` 和错误状态。
3. `DBStatusToString` 对已知状态和未知状态都返回非空字符串。
4. `DBErrorSet` 能写入 code、module、file、line 和 message。
5. `DBErrorSetf` 能格式化 message，并能处理空格式字符串。
6. `DBErrorClear` 能恢复到 `DB_OK` 初始状态。
7. `DBErrorPrint` 对 `NULL` err 或 `NULL` out 不崩溃。
8. `DB_SET_ERROR`、`DB_SET_ERRORF`、`DB_RETURN_ERROR`、`DB_TRY` 宏可以正常设置或传播状态。

## 兼容策略

旧接口先保留，不做一次性替换。`Lib/Error.h` / `Lib/Error.c` 仍然存在，旧代码可以继续使用；新代码应优先包含 `error/DBError.h`。

迁移时优先新增 `Try` 版本，而不是直接改变旧函数签名。例如旧接口继续保留：

```c
Buffer *BufferManagerPin(BufferManager *bufferManager, BlockID *blockId);
```

后续可以新增：

```c
DBStatus BufferManagerTryPin(BufferManager *bufferManager, BlockID *blockId, Buffer **out, DBError *err);
```

## 建议迁移顺序

1. `File` / `Page` / `Buffer`
2. `Parser` / `Planner`
3. `Record` / `Metadata`
4. `Transaction` / `Lock` / `Recovery`
5. `DBMS.c` / `main.c` 顶层统一打印错误
