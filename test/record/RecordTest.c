//
// Created by Lenovo on 2025/7/31.
//


#include <stddef.h>
#include <cmocka.h>
#include "Schema.h"
#include "Layout.h"
#include "RID.h"
#include "RecordPage.h"
#include "FileManager.h"
#include "LogManager.h"
#include "BufferManager.h"
#include "Transaction.h"

// 测试 Schema 基本功能
static void test_schema_basic(void **state) {
    (void) state;

    // 创建一个新的 Schema
    Schema *schema = SchemaInit();
    assert_non_null(schema);

    // 添加整数字段
    CString *nameField = CStringCreateFromCStr("id");
    SchemaAddIntField(schema, nameField);

    // 添加字符串字段
    CString *titleField = CStringCreateFromCStr("title");
    SchemaAddStringField(schema, titleField, 50);

    // 验证字段类型
    assert_int_equal(SchemaType(schema, nameField), FILE_INFO_CODE_INTEGER);
    assert_int_equal(SchemaType(schema, titleField), FILE_INFO_CODE_VARCHAR);

    // 验证字段长度
    assert_int_equal(SchemaLength(schema, titleField), 50);

    // 验证字段存在性
    assert_true(SchemaHasField(schema, nameField));
    assert_true(SchemaHasField(schema, titleField));

    CString *nonExistentField = CStringCreateFromCStr("nonexistent");
    assert_false(SchemaHasField(schema, nonExistentField));
    
    CStringDestroy(nameField);
    CStringDestroy(titleField);
    CStringDestroy(nonExistentField);

    SchemaFree(schema);
}

// 测试 Schema 的 Add 和 AddAll 功能
static void test_schema_add_operations(void **state) {
    (void) state;

    // 创建源 Schema
    Schema *sourceSchema = SchemaInit();
    CString *field1 = CStringCreateFromCStr("field1");
    CString *field2 = CStringCreateFromCStr("field2");

    SchemaAddIntField(sourceSchema, field1);
    SchemaAddStringField(sourceSchema, field2, 30);

    // 创建目标 Schema
    Schema *targetSchema = SchemaInit();

    // 测试 SchemaAdd
    SchemaAdd(targetSchema, field1, sourceSchema);
    assert_true(SchemaHasField(targetSchema, field1));
    assert_int_equal(SchemaType(targetSchema, field1), FILE_INFO_CODE_INTEGER);

    // 测试 SchemaAddAll
    SchemaAddAll(targetSchema, sourceSchema);
    assert_true(SchemaHasField(targetSchema, field2));
    assert_int_equal(SchemaType(targetSchema, field2), FILE_INFO_CODE_VARCHAR);
    assert_int_equal(SchemaLength(targetSchema, field2), 30);
    
    CStringDestroy(field1);
    CStringDestroy(field2);

    SchemaFree(sourceSchema);
    SchemaFree(targetSchema);
}

// 测试 Layout 基本功能
static void test_layout_basic(void **state) {
    (void) state;

    // 创建 Schema
    Schema *schema = SchemaInit();
    CString *idField = CStringCreateFromCStr("id");
    CString *nameField = CStringCreateFromCStr("name");
    CString *ageField = CStringCreateFromCStr("age");

    SchemaAddIntField(schema, idField);
    SchemaAddStringField(schema, nameField, 20);
    SchemaAddIntField(schema, ageField);

    // 创建 Layout
    Layout *layout = LayoutInit(schema, NULL, 0);
    assert_non_null(layout);

    // 验证偏移量
    int idOffset = LayoutOffset(layout, idField);
    int nameOffset = LayoutOffset(layout, nameField);
    int ageOffset = LayoutOffset(layout, ageField);
    
    CStringDestroy(idField);
    CStringDestroy(nameField);
    CStringDestroy(ageField);

    // 第一个字段应该在 sizeof(int) 的位置（跳过标志位）
    assert_int_equal(idOffset, sizeof(int));

    // name 字段应该在 id 字段后面
    assert_int_equal(nameOffset, sizeof(int) + sizeof(int));

    // age 字段应该在 name 字段后面
    assert_int_equal(ageOffset, sizeof(int) + sizeof(int) + 20 + sizeof(int));

    // 验证槽大小
    int expectedSlotSize = sizeof(int) + // 标志位
                           sizeof(int) + // id
                           20 + sizeof(int) + // name (length + size prefix)
                           sizeof(int); // age
    assert_int_equal(layout->SlotSize, expectedSlotSize);

    free(layout->offsets);
    free(layout);
    SchemaFree(schema);
}

// 测试 RID 功能
static void test_rid_basic(void **state) {
    (void) state;

    // 创建 RID
    RID *rid1 = RIDInit(1, 5);
    RID *rid2 = RIDInit(1, 5);
    RID *rid3 = RIDInit(2, 5);

    assert_non_null(rid1);
    assert_non_null(rid2);
    assert_non_null(rid3);

    // 测试相等性
    assert_true(RIDEqual(rid1, rid2));
    assert_false(RIDEqual(rid1, rid3));

    // 测试字符串表示
    char *ridStr = RIDToString(rid1);
    assert_non_null(ridStr);
    assert_string_equal(ridStr, "(1,5)");

    free(rid1);
    free(rid2);
    free(rid3);
    free(ridStr);
}

// 测试 RecordPage 基本功能
static void test_record_page_basic(void **state) {
    (void) state;

    // 设置测试环境
    FileManager *fileManager = FileManagerInit(CStringCreateFromCStr("RecordTest"), 400);
    LogManager *logManager = LogManagerInit(fileManager, CStringCreateFromCStr("record.log"));
    BufferManager *bufferManager = BufferManagerInit(fileManager, logManager, 8,NULL);
    Transaction *tx = TransactionInit(fileManager, logManager, bufferManager);

    BlockID *blockId = BlockIDInit(CStringCreateFromCStr("recordfile"), 0);

    // 创建 Schema 和 Layout
    Schema *schema = SchemaInit();
    CString *idField = CStringCreateFromCStr("id");
    CString *nameField = CStringCreateFromCStr("name");

    SchemaAddIntField(schema, idField);
    SchemaAddStringField(schema, nameField, 20);

    Layout *layout = LayoutInit(schema, NULL, 0);

    // 创建 RecordPage
    RecordPage *recordPage = RecordPageInit(tx, blockId, layout);
    assert_non_null(recordPage);

    // 格式化页面
    RecordPageFormat(recordPage);

    // 插入记录
    int slot = RecordPageInsertAfter(recordPage, -1);
    assert_true(slot >= 0);

    // 设置记录值
    RecordSetInt(recordPage, slot, idField, 123);
    RecordSetString(recordPage, slot, nameField, CStringCreateFromCStr("John Doe"));

    // 读取记录值
    int retrievedId = RecordPageGetInt(recordPage, slot, idField);
    char *retrievedNameStr = RecordPageGetString(recordPage, slot, nameField);
    CString *retrievedName = CStringCreateFromCStr(retrievedNameStr);

    assert_int_equal(retrievedId, 123);
    assert_string_equal(CStringGetPtr(retrievedName), "John Doe");

    // 测试下一条记录查找
    int nextSlot = RecordPageInsertAfter(recordPage, slot);
    if (nextSlot >= 0) {
        RecordSetInt(recordPage, nextSlot, idField, 456);

        int foundSlot = RecordPageNextAfter(recordPage, slot);
        assert_int_equal(foundSlot, nextSlot);

        int retrievedId2 = RecordPageGetInt(recordPage, foundSlot, idField);
        assert_int_equal(retrievedId2, 456);
    }

    // 测试记录删除
    RecordPageDelete(recordPage, slot);

    // 清理
    TransactionCommit(tx);
    free(layout->offsets);
    free(layout);
    CStringDestroy(idField);
    CStringDestroy(nameField);
    CStringDestroy(retrievedName);
    SchemaFree(schema);
}

// 测试 RecordPage 的槽管理
static void test_record_page_slot_management(void **state) {
    (void) state;

    // 设置测试环境
    FileManager *fileManager = FileManagerInit(CStringCreateFromCStr("SlotTest"), 400);
    LogManager *logManager = LogManagerInit(fileManager, CStringCreateFromCStr("slot.log"));
    BufferManager *bufferManager = BufferManagerInit(fileManager, logManager, 8,NULL);
    Transaction *tx = TransactionInit(fileManager, logManager, bufferManager);

    BlockID *blockId = BlockIDInit(CStringCreateFromCStr("slotfile"), 0);

    // 创建简单的 Schema
    Schema *schema = SchemaInit();
    CString *dataField = CStringCreateFromCStr("data");
    SchemaAddIntField(schema, dataField);

    Layout *layout = LayoutInit(schema, NULL, 0);
    RecordPage *recordPage = RecordPageInit(tx, blockId, layout);

    // 格式化页面
    RecordPageFormat(recordPage);

    // 测试多个槽的插入和删除
    int slots[5];
    int validSlots = 0;

    // 插入多条记录
    for (int i = 0; i < 5; i++) {
        int slot = RecordPageInsertAfter(recordPage, validSlots > 0 ? slots[validSlots-1] : -1);
        if (slot >= 0) {
            slots[validSlots] = slot;
            RecordSetInt(recordPage, slot, dataField, i * 10);
            validSlots++;
        } else {
            break; // 页面满了
        }
    }

    assert_true(validSlots >= 2); // 至少应该能插入2条记录

    // 验证记录值
    for (int i = 0; i < validSlots; i++) {
        int value = RecordPageGetInt(recordPage, slots[i], dataField);
        assert_int_equal(value, i * 10);
    }

    // 删除中间的记录
    if (validSlots >= 3) {
        RecordPageDelete(recordPage, slots[1]);

        // 尝试在删除的位置后插入新记录
        int newSlot = RecordPageInsertAfter(recordPage, slots[0]);
        if (newSlot >= 0) {
            RecordSetInt(recordPage, newSlot, dataField, 999);
            int newValue = RecordPageGetInt(recordPage, newSlot, dataField);
            assert_int_equal(newValue, 999);
        }
    }

    // 清理
    TransactionCommit(tx);
    free(layout->offsets);
    free(layout);
    SchemaFree(schema);
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_schema_basic),
            cmocka_unit_test(test_schema_add_operations),
            cmocka_unit_test(test_layout_basic),
            cmocka_unit_test(test_rid_basic),
            cmocka_unit_test(test_record_page_basic),
            cmocka_unit_test(test_record_page_slot_management),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}