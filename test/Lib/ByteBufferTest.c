#include <cmocka.h>
#include "ByteBuffer.h"

void test_buffer_allocation(void **state) {
    (void)state; // unused variable

    // 测试正常分配
    ByteBuffer* buffer = bufferAllocate(1024);
    assert_non_null(buffer);
    assert_int_equal(buffer->size, 1024);
    assert_int_equal(buffer->position, 0);
    assert_int_equal(buffer->limit, 1024);

    // 测试释放
    bufferFree(buffer);

    // 测试零大小分配
    buffer = bufferAllocate(0);
    assert_null(buffer);
}

void test_buffer_basic_operations(void **state) {
    (void)state; // unused variable

    ByteBuffer* buffer = bufferAllocate(256);
    assert_non_null(buffer);

    // 测试清空
    bufferClear(buffer);
    assert_int_equal(buffer->position, 0);
    assert_int_equal(buffer->limit, 256);

    // 测试翻转
    buffer->position = 100;
    bufferFlip(buffer);
    assert_int_equal(buffer->position, 0);
    assert_int_equal(buffer->limit, 100);

    // 测试压缩
    bufferClear(buffer);
    buffer->position = 50;
    bufferCompact(buffer);
    assert_int_equal(buffer->position, 256-50);
    assert_int_equal(buffer->limit, 256);

    bufferFree(buffer);
}

void test_buffer_read_write(void **state) {
    (void)state; // unused variable

    ByteBuffer* buffer = bufferAllocate(128);
    assert_non_null(buffer);

    // 测试基本写入
    ByteBufferError err = bufferPut(buffer, 0xAB);
    assert_int_equal(err, BYTEBUFFER_OK);
    assert_int_equal(buffer->position, 1);

    // 测试short写入
    err = bufferPutShort(buffer, 0x1234);
    assert_int_equal(err, BYTEBUFFER_OK);
    assert_int_equal(buffer->position, 3);

    // 测试int写入
    err = bufferPutInt(buffer, 0x56789ABC);
    assert_int_equal(err, BYTEBUFFER_OK);
    assert_int_equal(buffer->position, 7);

    // 测试long写入
    err = bufferPutLong(buffer, 0x1122334455667788);
    assert_int_equal(err, BYTEBUFFER_OK);
    assert_int_equal(buffer->position, 15);

    // 重置位置准备读取
    buffer->position = 0;

    // 测试读取
    uint8_t byte_val;
    err = bufferGet(buffer, &byte_val);
    assert_int_equal(err, BYTEBUFFER_OK);
    assert_int_equal(byte_val, 0xAB);

    uint16_t short_val;
    err = bufferGetShort(buffer, &short_val);
    assert_int_equal(err, BYTEBUFFER_OK);
    assert_int_equal(short_val, 0x1234);

    int32_t int_val;
    err = bufferGetInt(buffer, &int_val);
    assert_int_equal(err, BYTEBUFFER_OK);
    assert_int_equal(int_val, 0x56789ABC);

    uint64_t long_val;
    err = bufferGetLong(buffer, &long_val);
    assert_int_equal(err, BYTEBUFFER_OK);
    assert_int_equal(long_val, 0x1122334455667788);

    bufferFree(buffer);
}

void test_buffer_position_operations(void **state) {
    (void)state; // unused variable

    ByteBuffer* buffer = bufferAllocate(64);
    assert_non_null(buffer);

    // 测试指定位置写入
    ByteBufferError err = bufferPutIntPosition(buffer, 10, 0x12345678);
    assert_int_equal(err, BYTEBUFFER_OK);
    assert_int_equal(buffer->position, 14);

    // 测试指定位置读取
    int32_t read_val;
    err = bufferGetIntPosition(buffer, 10, &read_val);
    assert_int_equal(err, BYTEBUFFER_OK);
    assert_int_equal(read_val, 0x12345678);

    // 测试设置位置
    uint64_t new_pos = bufferPosition(buffer, 20);
    assert_int_equal(new_pos, 20);
    assert_int_equal(buffer->position, 20);

    // 测试越界位置设置
    new_pos = bufferPosition(buffer, 100);
    assert_int_equal(new_pos, 0);

    bufferFree(buffer);
}

void test_buffer_bytes_operations(void **state) {
    (void)state; // unused variable

    ByteBuffer* buffer = bufferAllocate(128);
    assert_non_null(buffer);

    // 准备测试数据
    uint8_t test_data[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                             0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};

    // 测试批量写入
    ByteBufferError err = bufferPutBytes(buffer, test_data,  16);
    assert_int_equal(err, BYTEBUFFER_OK);
    assert_int_equal(buffer->position, 16);

    // 测试指定位置批量写入
    err = bufferPutBytesPosition(buffer, 32, test_data, 16);
    assert_int_equal(err, BYTEBUFFER_OK);

    // 测试批量读取
    uint8_t read_data[16];
    buffer->position = 0;
    err = bufferGetBytes(buffer, read_data, 16);
    assert_int_equal(err, BYTEBUFFER_OK);
    assert_memory_equal(test_data, read_data, 16);

    // 测试指定位置批量读取
    err = bufferGetBytesPosition(buffer, 32, read_data, 16);
    assert_int_equal(err, BYTEBUFFER_OK);
    assert_memory_equal(test_data, read_data, 16);

    bufferFree(buffer);
}

void test_error_handling(void **state) {
    (void)state; // unused variable

    ByteBuffer* buffer = bufferAllocate(16);
    assert_non_null(buffer);

    // 测试NULL缓冲区
    ByteBufferError err = bufferPut(NULL, 0xAA);
    assert_int_equal(err, BYTEBUFFER_ERROR_NULL);

    // 测试越界写入
    buffer->position = buffer->limit - 1;
    err = bufferPutShort(buffer, 0x1234);
    assert_int_equal(err, BYTEBUFFER_ERROR_BOUNDS);

    // 测试NULL数据指针
    uint8_t* null_data = NULL;
    err = bufferPutBytes(buffer, null_data, 10);
    assert_int_equal(err, BYTEBUFFER_ERROR_NULL);

    bufferFree(buffer);
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_buffer_allocation),
            cmocka_unit_test(test_buffer_basic_operations),
            cmocka_unit_test(test_buffer_read_write),
            cmocka_unit_test(test_buffer_position_operations),
            cmocka_unit_test(test_buffer_bytes_operations),
            cmocka_unit_test(test_error_handling),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
