#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdint.h>
#include "StatInfo.h"
#include <stdlib.h>

// 测试 StatInfoInit 函数
static void test_stat_info_init(void **state) {
    (void)state;

    // 初始化一个 StatInfo 实例
    int numBlocks = 100;
    int numRecs = 1000;
    StatInfo *si = StatInfoInit(numBlocks, numRecs);

    // 验证实例是否正确初始化
    assert_non_null(si);
    assert_int_equal(si->numBlocks, numBlocks);
    assert_int_equal(si->numRecs, numRecs);
    
    // 释放内存
    free(si);
}

// 测试 StatInfoBlocksAccessed 函数
static void test_stat_info_blocks_accessed(void **state) {
    (void)state;

    // 初始化一个 StatInfo 实例
    int numBlocks = 50;
    StatInfo *si = StatInfoInit(numBlocks, 500);

    // 验证获取块数量的函数
    assert_int_equal(StatInfoBlocksAccessed(si), numBlocks);
    
    // 释放内存
    free(si);
}

// 测试 StatInfoRecordsOutput 函数
static void test_stat_info_records_output(void **state) {
    (void)state;

    // 初始化一个 StatInfo 实例
    int numRecs = 2000;
    StatInfo *si = StatInfoInit(20, numRecs);

    // 验证获取记录数量的函数
    assert_int_equal(StatInfoRecordsOutput(si), numRecs);
    
    // 释放内存
    free(si);
}

// 测试 StatInfoDistinctValues 函数
static void test_stat_info_distinct_values(void **state) {
    (void)state;

    // 初始化一个 StatInfo 实例
    StatInfo *si = StatInfoInit(30, 3000);

    // 测试获取不同字段的不同值数量
    // 注意：这里假设 StatInfoDistinctValues 函数实现了基本的功能
    int distinctValues1 = StatInfoDistinctValues(si, "id");
    int distinctValues2 = StatInfoDistinctValues(si, "name");
    
    // 验证返回值是整数
    assert_true(distinctValues1 >= 0);
    assert_true(distinctValues2 >= 0);
    
    // 释放内存
    free(si);
}

// 主函数：注册所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_stat_info_init),
            cmocka_unit_test(test_stat_info_blocks_accessed),
            cmocka_unit_test(test_stat_info_records_output),
            cmocka_unit_test(test_stat_info_distinct_values),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}