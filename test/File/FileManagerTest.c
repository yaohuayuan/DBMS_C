//
// Created by Lenovo on 2025/6/24.
//
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "FileManager.h"
#include "BlockID.h"
#include "Page.h"
#include "CString.h"

// 假设这些头文件已经正确包含了 FileManager、BlockID、Page 等结构体的定义

static void test_FileManager_operations(void **state) {
    (void) state; // Unused parameter

    // 初始化测试数据
    CString *str = CStringCreateFromCStr("FileTest");
    FileManager *fileManager = FileManagerInit(str, 400);
    BlockID* blockId = BlockIDInit(CStringCreateFromCStr("testFile"), 2);
    Page* p1 = PageInit(fileManager->blockSize);

    // 设置字符串数据到页面
    PageSetString(p1, 40, CStringCreateFromCStr("123456789"));
    int length = PageMaxLength(CStringCreateFromCStr("123456789"));
    int pos2 = 40 + length;

    // 设置整数数据到页面
    PageSetInt(p1, pos2, 54);

    // 写入文件
    FileManagerWrite(fileManager, blockId, p1);

    // 读取文件
    Page* p2 = PageInit(fileManager->blockSize);
    FileManagerRead(fileManager, blockId, p2);
    int data = PageGetInt(p1, pos2);
    CString*cs = PageGetString(p2,40 );
    printf("%d,%s",data, CStringGetPtr(cs));
//     断言，验证数据是否正确写入并读取
    assert_int_equal(PageGetInt(p2, pos2), 54);
    printf("%s",CStringGetPtr(PageGetString(p2,40 )));
    assert_string_equal(CStringGetPtr(PageGetString(p2,40 )), "123456789");
    FileManagerDestroy(fileManager);
}

static void test_FileManager_Init_Failure(void **state) {
    (void) state;

    // 假设FileManagerInit会在初始化失败时返回NULL
    FileManager *fileManager = FileManagerInit(NULL, 400);
    assert_null(fileManager);
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_FileManager_operations),
            cmocka_unit_test(test_FileManager_Init_Failure),
    };

    // 运行单元测试
    return cmocka_run_group_tests(tests, NULL, NULL);
}
