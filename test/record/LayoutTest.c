#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "Layout.h"
#include "Schema.h"
#include "CString.h"

// 测试Layout初始化
static void test_layout_init(void **state) {
    Schema *schema = SchemaInit();
    CString *idField = CStringCreateFromCStr("id");
    CString *nameField = CStringCreateFromCStr("name");
    
    SchemaAddIntField(schema, idField);
    SchemaAddStringField(schema, nameField, 20);
    
    // 创建字段偏移量映射
    map_int_t offsets;
    map_init(&offsets);
    map_set(&offsets, CStringGetPtr(idField), 0);
    map_set(&offsets, CStringGetPtr(nameField), 4);
    
    int slot_size = 24; // 4字节id + 20字节name
    Layout *layout = LayoutInit(schema, &offsets, slot_size);
    
    assert_non_null(layout);
    assert_non_null(layout->schema);
    assert_non_null(layout->offsets);
    assert_int_equal(layout->SlotSize, slot_size);
    
    // 清理资源
    free(layout);
    map_deinit(&offsets);
    CStringDestroy(idField);
    CStringDestroy(nameField);
    SchemaFree(schema);
}

// 测试获取字段偏移量
static void test_layout_offset(void **state) {
    Schema *schema = SchemaInit();
    CString *idField = CStringCreateFromCStr("id");
    CString *nameField = CStringCreateFromCStr("name");
    
    SchemaAddIntField(schema, idField);
    SchemaAddStringField(schema, nameField, 20);
    
    // 创建字段偏移量映射
    map_int_t offsets;
    map_init(&offsets);
    map_set(&offsets, CStringGetPtr(idField), 0);
    map_set(&offsets, CStringGetPtr(nameField), 4);
    
    int slot_size = 24;
    Layout *layout = LayoutInit(schema, &offsets, slot_size);
    
    assert_int_equal(LayoutOffset(layout, idField), 0);
    assert_int_equal(LayoutOffset(layout, nameField), 4);
    
    // 清理资源
    free(layout);
    map_deinit(&offsets);
    CStringDestroy(idField);
    CStringDestroy(nameField);
    SchemaFree(schema);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_layout_init),
        cmocka_unit_test(test_layout_offset),
    };
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}
