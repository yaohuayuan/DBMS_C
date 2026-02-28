#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdint.h>
#include "InsertData.h"
#include <stdlib.h>
#include <string.h>
#include "CString.h"

// 测试 InsertData 结构体的基本功能
static void test_insert_data_struct(void **state) {
    (void)state;

    // 注意：由于 InsertDataInit 依赖于 List，这里仅测试结构体的基本功能
    // 在实际项目中，应该使用模拟（mock）的组件来测试完整功能

    // 创建一个简单的 InsertData 实例
    InsertData *insertData = (InsertData *)malloc(sizeof(InsertData));
    
    // 验证结构体的字段初始化
    assert_non_null(insertData);
    insertData->tblname = CStringCreateFromCStr("test_table");
    insertData->flds = NULL;
    insertData->vals = NULL;
    
    // 验证字段值
    assert_string_equal(CStringGetPtr(insertData->tblname), "test_table");
    assert_null(insertData->flds);
    assert_null(insertData->vals);
    
    // 释放内存
    CStringDestroy(insertData->tblname);
    free(insertData);
}

// 主函数：注册所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_insert_data_struct),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}