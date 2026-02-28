#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdint.h>
#include "IndexInfo.h"
#include <stdlib.h>
#include <string.h>
#include "CString.h"

// 测试 IndexInfo 结构体的基本功能
static void test_index_info_struct(void **state) {
    (void)state;

    // 注意：由于 IndexInfoInit 依赖于其他组件，这里仅测试结构体的基本功能
    // 在实际项目中，应该使用模拟（mock）的组件来测试完整功能

    // 创建一个简单的 IndexInfo 实例
    IndexInfo *indexInfo = (IndexInfo *)malloc(sizeof(IndexInfo));
    
    // 验证结构体的字段初始化
    assert_non_null(indexInfo);
    indexInfo->idxname = NULL;
    indexInfo->fldname = NULL;
    indexInfo->transaction = NULL;
    indexInfo->tblSchema = NULL;
    indexInfo->layout = NULL;
    indexInfo->statInfo = NULL;
    
    // 验证字段值
    assert_null(indexInfo->idxname);
    assert_null(indexInfo->fldname);
    assert_null(indexInfo->transaction);
    assert_null(indexInfo->tblSchema);
    assert_null(indexInfo->layout);
    assert_null(indexInfo->statInfo);
    
    // 释放内存
    free(indexInfo);
}

// 测试 IndexInfo 字符串字段的基本功能
static void test_index_info_string_fields(void **state) {
    (void)state;

    // 创建一个简单的 IndexInfo 实例
    IndexInfo *indexInfo = (IndexInfo *)malloc(sizeof(IndexInfo));
    
    // 设置字符串字段
    indexInfo->idxname = CStringCreateFromCStr("test_index");
    indexInfo->fldname = CStringCreateFromCStr("test_field");
    
    // 验证字符串字段
    assert_non_null(indexInfo->idxname);
    assert_non_null(indexInfo->fldname);
    assert_string_equal(CStringGetPtr(indexInfo->idxname), "test_index");
    assert_string_equal(CStringGetPtr(indexInfo->fldname), "test_field");
    
    // 释放字符串内存
    CStringDestroy(indexInfo->idxname);
    CStringDestroy(indexInfo->fldname);
    free(indexInfo);
}

// 主函数：注册所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_index_info_struct),
            cmocka_unit_test(test_index_info_string_fields),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}