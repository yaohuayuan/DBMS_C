#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "Schema.h"
#include "CString.h"

// 测试Schema初始化
static void test_schema_init(void **state) {
    Schema *schema = SchemaInit();
    
    assert_non_null(schema);
    assert_non_null(schema->MapFileInfo);
    
    // 清理资源
    SchemaFree(schema);
}

// 测试向Schema添加字段
static void test_schema_add_field(void **state) {
    Schema *schema = SchemaInit();
    CString *idField = CStringCreateFromCStr("id");
    CString *nameField = CStringCreateFromCStr("name");
    CString *nonExistentField = CStringCreateFromCStr("non_existent");
    
    SchemaAddIntField(schema, idField);
    SchemaAddStringField(schema, nameField, 20);
    
    assert_true(SchemaHasField(schema, idField));
    assert_true(SchemaHasField(schema, nameField));
    assert_false(SchemaHasField(schema, nonExistentField));
    
    // 清理资源
    CStringDestroy(idField);
    CStringDestroy(nameField);
    CStringDestroy(nonExistentField);
    SchemaFree(schema);
}

// 测试Schema字段类型和长度
static void test_schema_field_type_length(void **state) {
    Schema *schema = SchemaInit();
    CString *idField = CStringCreateFromCStr("id");
    CString *nameField = CStringCreateFromCStr("name");
    
    SchemaAddIntField(schema, idField);
    SchemaAddStringField(schema, nameField, 20);
    
    assert_int_equal(SchemaType(schema, idField), FILE_INFO_CODE_INTEGER);
    assert_int_equal(SchemaLength(schema, idField), 4); // 假设整数类型长度为4
    assert_int_equal(SchemaType(schema, nameField), FILE_INFO_CODE_VARCHAR);
    assert_int_equal(SchemaLength(schema, nameField), 20);
    
    // 清理资源
    CStringDestroy(idField);
    CStringDestroy(nameField);
    SchemaFree(schema);
}

// 测试Schema添加所有字段
static void test_schema_add_all(void **state) {
    Schema *schema1 = SchemaInit();
    CString *idField = CStringCreateFromCStr("id");
    CString *nameField = CStringCreateFromCStr("name");
    
    SchemaAddIntField(schema1, idField);
    SchemaAddStringField(schema1, nameField, 20);
    
    Schema *schema2 = SchemaInit();
    CString *ageField = CStringCreateFromCStr("age");
    
    SchemaAddIntField(schema2, ageField);
    SchemaAddAll(schema2, schema1);
    
    assert_true(SchemaHasField(schema2, idField));
    assert_true(SchemaHasField(schema2, nameField));
    assert_true(SchemaHasField(schema2, ageField));
    
    // 清理资源
    CStringDestroy(idField);
    CStringDestroy(nameField);
    CStringDestroy(ageField);
    SchemaFree(schema1);
    SchemaFree(schema2);
}

// 测试Schema添加指定字段
static void test_schema_add(void **state) {
    Schema *schema1 = SchemaInit();
    CString *idField = CStringCreateFromCStr("id");
    CString *nameField = CStringCreateFromCStr("name");
    
    SchemaAddIntField(schema1, idField);
    SchemaAddStringField(schema1, nameField, 20);
    
    Schema *schema2 = SchemaInit();
    SchemaAdd(schema2, idField, schema1);
    
    assert_true(SchemaHasField(schema2, idField));
    assert_false(SchemaHasField(schema2, nameField));
    
    // 清理资源
    CStringDestroy(idField);
    CStringDestroy(nameField);
    SchemaFree(schema1);
    SchemaFree(schema2);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_schema_init),
        cmocka_unit_test(test_schema_add_field),
        cmocka_unit_test(test_schema_field_type_length),
        cmocka_unit_test(test_schema_add_all),
        cmocka_unit_test(test_schema_add),
    };
    
    return cmocka_run_group_tests(tests, NULL, NULL);
}
