//
// Created by Lenovo on 2025/7/3.
//
#include "LogManager.h"
#include "cmocka.h"
#include "time.h"
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "LogManager.h"
#include "time.h"
#include "sys/stat.h"   // mkdir

/* ---------- 测试夹具 ---------- */

typedef struct {
    FileManager *fm;
    LogManager  *lm;
    CString     *fileName;
} TestCtx;

/* 在每个用例前创建独立的日志文件与目录 */
static int set_up(void **state) {
    TestCtx *ctx = malloc(sizeof(TestCtx));
    assert_non_null(ctx);

    /* 清理可能存在的测试目录和文件 */
    char dirPath[256];
    snprintf(dirPath, sizeof(dirPath), "Logtest");
    // 尝试删除目录中的所有文件
    WIN32_FIND_DATAA findFileData;
    char searchPath[256];
    snprintf(searchPath, sizeof(searchPath), "%s\\*", dirPath);
    HANDLE hFind = FindFirstFileA(searchPath, &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
                char filePath[256];
                snprintf(filePath, sizeof(filePath), "%s\\%s", dirPath, findFileData.cFileName);
                DeleteFileA(filePath);
            }
        } while (FindNextFileA(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
    // 删除空目录
    RemoveDirectoryA(dirPath);

    /* 准备日志目录 */
    ctx->fm = FileManagerInit(CStringCreateFromCStr("Logtest"), 400);

    /* 生成唯一文件名 */
    char fileNameBuf[128];
    time_t t  = time(NULL);
    struct tm tm_now;
    gmtime_s(&tm_now, &t);
    snprintf(fileNameBuf, sizeof(fileNameBuf),
             "log_%04d%02d%02d_%02d%02d%02d",
             tm_now.tm_year+1900, tm_now.tm_mon+1, tm_now.tm_mday,
             tm_now.tm_hour, tm_now.tm_min, tm_now.tm_sec);
    ctx->fileName = CStringCreateFromCStr(fileNameBuf);
    ctx->lm = LogManagerInit(ctx->fm, ctx->fileName);
    *state = ctx;
    return 0;
}

/* 用例结束后释放资源 */
static int tear_down(void **state) {
    TestCtx *ctx = *state;
    /* 按需增加文件删除逻辑 */
    // 删除日志文件
    char *fileName = CStringGetPtr(ctx->fileName);
    // 尝试删除可能的路径
    char path1[256];
    char path2[256];
    snprintf(path1, sizeof(path1), "Logtest\\%s", fileName);
    snprintf(path2, sizeof(path2), "%s", fileName);
    DeleteFileA(path1);
    DeleteFileA(path2);
    // 清理目录
    char dirPath[256];
    snprintf(dirPath, sizeof(dirPath), "Logtest");
    RemoveDirectoryA(dirPath);
    FileManagerDestroy(ctx->fm);      /* 如有此接口 */
    free(ctx);
    return 0;
}

/* ---------- 帮助函数 ---------- */

/* 生成一条 [string,n] 记录，与 PageSetBytes 格式一致 */
static ByteBuffer* make_record(CString *str, int n) {
    CString *cs   = CStringCreateFromCString(str);
    int spos      = 0;
    int nops      = spos + PageMaxLength(cs);
    int length    = nops + sizeof(int);

    ByteBuffer *buf = bufferAllocate(length + sizeof(int));
    Page *pg = PageInitBytes(buf);

    PageSetString(pg, 0, cs);
    PageSetInt(pg, nops, n);

    return buf;
}

/* 从日志记录反解出 <string,int> */
static void parse_record(ByteBuffer *bb, char *outStr, size_t outSz, int *outN) {
    Page *pg = PageInitBytes(bb);
    CString *cs = PageGetString(pg, 0);
    int offs  = PageMaxLength(cs);
    *outN     = PageGetInt(pg, offs);
    strncpy(outStr, CStringGetPtr(cs), outSz-1);
    outStr[outSz-1] = '\0';
}

/* ---------- TEST CASE 1：空日志 ---------- */
static void test_empty_log(void **state) {
    TestCtx *ctx = *state;

    LogIterator *it = LogManager2LogManager(ctx->lm);
    assert_false(LogIteratorHasNext(it));   /* 应该没有记录 */
}

/* ---------- TEST CASE 2：写入并遍历 ---------- */
static void test_append_and_iterate(void **state) {
    TestCtx *ctx = *state;
    const int N = 100;

    /* 写 N 条记录 */
    for (int i = 0; i < N; ++i) {
        char tmp[32];
        snprintf(tmp, sizeof(tmp), "record%d", i);
        ByteBuffer *rec = make_record(CStringCreateFromCStr(tmp), i);
        int lsn = LogManagerAppend(ctx->lm, rec->data, rec->size);
        assert_int_equal(lsn, i+1);     /* LSN 连续自增 */
    }

    /* 迭代读取并校验 */
    LogIterator *it = LogManager2LogManager(ctx->lm);
    int count = 0;
    while (LogIteratorHasNext(it)) {
        ByteBuffer *bb = LogIteratorNext(it);
        char s[32];
        int  n;
        parse_record(bb, s, sizeof(s), &n);

        /* 断言内容匹配 */
        char expect[32];
        snprintf(expect, sizeof(expect), "record%d", n);
        assert_string_equal(s, expect);

        ++count;
    }
    assert_int_equal(count, N);
}

/* ---------- TEST CASE 3：Flush LSN ---------- */
static void test_flush_lsn(void **state) {
    TestCtx *ctx = *state;

    /* 写 10 条记录，LSN 10 */
    for (int i = 1; i <= 10; ++i) {
        ByteBuffer *rec = make_record(CStringCreateFromCStr("x"), i);
        LogManagerAppend(ctx->lm, rec->data, rec->size);
    }

    LogManagerFlushLSN(ctx->lm, 10);
    assert_int_equal(ctx->lm->LastSavedLSN, 10);
}
static void test_partial_flush(void **state) {
    TestCtx *ctx = *state;
    const int FLUSH_N = 5;
    const int TOTAL_N = 10;

    // 1. 写入前半部分并 flush
    for (int i = 0; i < FLUSH_N; ++i) {
        char tmp[32];
        snprintf(tmp, sizeof(tmp), "persisted%d", i);
        ByteBuffer *rec = make_record(CStringCreateFromCStr(tmp), i);
        LogManagerAppend(ctx->lm, rec->data, rec->size);
    }
    LogManagerFlushLSN(ctx->lm, FLUSH_N);
    assert_int_equal(ctx->lm->LastSavedLSN, FLUSH_N);

    // 2. 写入后半部分，但不 flush
    for (int i = FLUSH_N; i < TOTAL_N; ++i) {
        char tmp[32];
        snprintf(tmp, sizeof(tmp), "lost%d", i);
        ByteBuffer *rec = make_record(CStringCreateFromCStr(tmp), i);
        LogManagerAppend(ctx->lm, rec->data, rec->size);
    }

    // 3. 重建 LogManager（模拟系统重启）
    LogManager *recoveredLM = LogManagerInit(ctx->fm, ctx->fileName);
    LogIterator *it = LogManager2LogManager(recoveredLM);

    int count = 0;
    while (LogIteratorHasNext(it)) {
        ByteBuffer *bb = LogIteratorNext(it);
        char s[32];
        int  n;
        parse_record(bb, s, sizeof(s), &n);

        // 断言只能读到前半部分
        char expect[32];
        snprintf(expect, sizeof(expect), "persisted%d", n);
        assert_string_equal(s, expect);
        ++count;
        // 只读取 FLUSH_N 条记录
        if (count >= FLUSH_N) {
            break;
        }
    }

    assert_int_equal(count, FLUSH_N);
}
/* ---------- main ---------- */

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test_setup_teardown(test_empty_log,          set_up, tear_down),
            cmocka_unit_test_setup_teardown(test_append_and_iterate, set_up, tear_down),
            cmocka_unit_test_setup_teardown(test_flush_lsn,          set_up, tear_down),
            cmocka_unit_test_setup_teardown(test_partial_flush, set_up, tear_down),
            };
    return cmocka_run_group_tests(tests, NULL, NULL);
}