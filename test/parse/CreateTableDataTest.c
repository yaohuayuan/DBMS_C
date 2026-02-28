#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include <stdint.h>
#include "CreateTableData.h"
#include <stdlib.h>
#include <string.h>
#include "CString.h"

// 测试 CreateTableData 结构体的基本功能
static void test_create_table_data_struct(void **state) {
    (void)state;

    // 注意：由于 CreateTableDataInit 依赖于 Schema，这里仅测试结构体的基本功能
    // 在实际项目中，应该使用模拟（mock）的组件来测试完整功能

    // 创建一个简单的 CreateTableData 实例
    CreateTableData *createTableData = (CreateTableData *)malloc(sizeof(CreateTableData));
    
    // 验证结构体的字段初始化
    assert_non_null(createTableData);
    createTableData->tblname = CStringCreateFromCStr("test_table");
    createTableData->schema = NULL;
    
    // 验证字段值
    assert_string_equal(CStringGetPtr(createTableData->tblname), "test_table");
    assert_null(createTableData->schema);
    
    // 释放内存
    CStringDestroy(createTableData->tblname);
    free(createTableData);
}

// 主函数：注册所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_create_table_data_struct),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}