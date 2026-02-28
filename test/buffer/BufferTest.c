//
// Created by Lenovo on 2025/7/17.
//
#include "Buffer.h"
#include "cmocka.h"

/* setup */
int setup(void **state){
    CString *dbName = CStringCreateFromCStr("BufferTest");
    CString *fileName = CStringCreateFromCStr("log");

    FileManager *fileManager = FileManagerInit(dbName, 400);
    LogManager *lm = LogManagerInit(fileManager, fileName);

    Buffer *buffer = BufferInit(fileManager, lm);
    *state = buffer;
    return 0;
}

/* teardown */
int teardown(void **state){
    Buffer *buffer = (Buffer *)(*state);
    if(buffer){

        free(buffer);
    }
    return 0;
}

/* 基础功能测试 */
void test_buffer_basic(void **state) {
    Buffer *buffer = (Buffer *)(*state);

    /* 初始状态 */
    assert_int_equal(buffer->pins, 0);
    assert_false(BufferIsPinned(buffer));

    /* Pin / Unpin */
    BufferPin(buffer);
    assert_int_equal(buffer->pins, 1);
    assert_true(BufferIsPinned(buffer));

    BufferUnPin(buffer);
    assert_int_equal(buffer->pins, 0);
    assert_false(BufferIsPinned(buffer));

    /* 设置修改状态 */
    BufferSetModified(buffer, 123, 456);
    assert_int_equal(buffer->txNum, 123);
    assert_int_equal(buffer->lsn, 456);

    /* 构造合法 BlockID */
    CString *tbl = CStringCreateFromCStr("test.tbl");
    BlockID *bid = BlockIDInit(tbl, 0);

    BufferAssignToBlock(buffer, bid);
    assert_non_null(buffer->blockId);

    /* flush 后应清理 dirty 标记 */
    BufferFlush(buffer);
    assert_int_equal(buffer->txNum, -1);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(
            test_buffer_basic,
            setup,
            teardown
        ),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
