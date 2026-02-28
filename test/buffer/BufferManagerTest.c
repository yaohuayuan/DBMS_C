//
// Created by Lenovo on 2025/7/17.
//
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "BufferManager.h"
#include "Buffer.h"
#include "FileManager.h"
#include "LogManager.h"
#include "BlockId.h"
#include "CString.h"
#include"LRU/LRUPolicy.h"
/* ---------- 测试工具函数 ---------- */

static FileManager* create_dummy_file_manager(void) {
    CString *dbDir = CStringCreateFromCStr("testdb_bufmgr");
    return FileManagerInit(dbDir, 512);
}

static LogManager* create_dummy_log_manager(FileManager *fm) {
    CString *logFile = CStringCreateFromCStr("log_bufmgr");
    return LogManagerInit(fm, logFile);
}

static void destroy_test_env(BufferManager *bm,
                             FileManager *fm,
                             LogManager *lm) {
    /* TODO：如果你实现了 destroy 函数，在这里调用 */
    (void)bm;
    (void)fm;
    (void)lm;
}

/* ---------- 测试环境 ---------- */

static BufferManager *bm = NULL;
static FileManager *fm = NULL;
static LogManager *lm = NULL;
static const int bufferCount = 5;

static int setup(void **state) {
    fm = create_dummy_file_manager();
    lm = create_dummy_log_manager(fm);
    ReplacementPolicy *replacementPolicy = LRUPolicyCreate(bufferCount);
    bm = BufferManagerInit(fm, lm, bufferCount,replacementPolicy);

    assert_non_null(bm);
    *state = bm;
    return 0;
}

static int teardown(void **state) {
    (void)state;
    destroy_test_env(bm, fm, lm);
    bm = NULL;
    fm = NULL;
    lm = NULL;
    return 0;
}

/* ---------- Tests ---------- */

static void test_init(void **state) {
    BufferManager *bm = *state;
    assert_int_equal(bm->bufferSize, bufferCount);
    assert_int_equal(bm->numAvailable, bufferCount);
    assert_non_null(bm->bufferPool);
}

static void test_choose_unpinned_buffer(void **state) {
    BufferManager *bm = *state;

    Buffer *buf = BufferManagerChooseUnPinnedBuffer(bm);
    assert_non_null(buf);
    assert_false(BufferIsPinned(buf));
}

static void test_find_existing_buffer(void **state) {
    BufferManager *bm = *state;

    CString *fname = CStringCreateFromCStr("blockA");
    BlockID *block = BlockIDInit(fname, 1);

    assert_null(BufferManagerFindExistingBuffer(bm, block));

    Buffer *pinned = BufferManagerPin(bm, block);
    assert_non_null(pinned);

    Buffer *found = BufferManagerFindExistingBuffer(bm, block);
    assert_ptr_equal(found, pinned);

    BufferManagerUnpin(bm, pinned);
    BlockIDDestroy(block);
}

static void test_try_to_pin(void **state) {
    BufferManager *bm = *state;

    CString *fname = CStringCreateFromCStr("blockB");
    BlockID *block = BlockIDInit(fname, 2);

    Buffer *buf = BufferManagerTryToPin(bm, block);
    assert_non_null(buf);
    assert_true(BufferIsPinned(buf));
    assert_int_equal(bm->numAvailable, bufferCount - 1);
    assert_true(BlockIDEqual(buf->blockId, block));

    BufferManagerUnpin(bm, buf);
    BlockIDDestroy(block);
}

static void test_pin_and_unpin(void **state) {
    BufferManager *bm = *state;

    CString *fname = CStringCreateFromCStr("blockC");
    BlockID *block = BlockIDInit(fname, 3);

    Buffer *buf = BufferManagerPin(bm, block);
    assert_non_null(buf);
    assert_true(BufferIsPinned(buf));

    BufferManagerUnpin(bm, buf);
    assert_false(BufferIsPinned(buf));
    assert_int_equal(bm->numAvailable, bufferCount);

    BlockIDDestroy(block);
}

/* ---------- main ---------- */

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(test_init, setup, teardown),
        cmocka_unit_test_setup_teardown(test_choose_unpinned_buffer, setup, teardown),
        cmocka_unit_test_setup_teardown(test_find_existing_buffer, setup, teardown),
        cmocka_unit_test_setup_teardown(test_try_to_pin, setup, teardown),
        cmocka_unit_test_setup_teardown(test_pin_and_unpin, setup, teardown),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
