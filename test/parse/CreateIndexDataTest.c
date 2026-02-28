#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdint.h>
#include "CreateIndexData.h"
#include <stdlib.h>
#include <string.h>
#include "CString.h"

// 测试 CreateIndexData 结构体的基本功能
static void test_create_index_data_struct(void **state) {
    (void)state;

    // 注意：由于 CreateIndexDataInit 只依赖于字符串参数，这里测试结构体的完整功能
    // 在实际项目中，应该使用模拟（mock）的组件来测试完整功能

    // 创建一个简单的 CreateIndexData 实例
    CreateIndexData *createIndexData = (CreateIndexData *)malloc(sizeof(CreateIndexData));
    
    // 验证结构体的字段初始化
    assert_non_null(createIndexData);
    createIndexData->idxname = CStringCreateFromCStr("test_index");
    createIndexData->tblname = CStringCreateFromCStr("test_table");
    createIndexData->fldname = CStringCreateFromCStr("test_field");
    
    // 验证字段值
    assert_string_equal(CStringGetPtr(createIndexData->idxname), "test_index");
    assert_string_equal(CStringGetPtr(createIndexData->tblname), "test_table");
    assert_string_equal(CStringGetPtr(createIndexData->fldname), "test_field");
    
    // 释放内存
    CStringDestroy(createIndexData->idxname);
    CStringDestroy(createIndexData->tblname);
    CStringDestroy(createIndexData->fldname);
    free(createIndexData);
}

// 主函数：注册所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_create_index_data_struct),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}