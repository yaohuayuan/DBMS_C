#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include "Page.h"

// 测试 PageInit 和基本属性
static void test_PageInit(void **state) {
    Page *p = PageInit(1024);
    assert_non_null(p);
    assert_int_equal(p->buffer->size, 1024);
    PageDestroy(p);
}

// 测试整数设置和获取
static void test_PageInt(void **state) {
    Page *p = PageInit(1024);

    // 测试正常整数
    PageSetInt(p, 0, 0);
    int val;
    val = PageGetInt(p, 0);
    assert_int_equal(val, 0);

    // 测试边界值
    PageSetInt(p, 508, INT_MAX);
    val = PageGetInt(p, 508);
    assert_int_equal(val, INT_MAX);

    PageDestroy(p);
}

// 测试字符串设置和获取
static void test_PageString(void **state) {
    Page *p = PageInit(1024);

    // 正常字符串
    CString *s1 = CStringCreateFromCStr("Hello, DBMS!");
    PageSetString(p, 100, s1);
    CString *retrieved = PageGetString(p, 100);
    assert_string_equal(CStringGetPtr(retrieved), "Hello, DBMS!");
    CStringDestroy(s1);
    CStringDestroy(retrieved);

    // 空字符串
    CString *s2 = CStringCreateFromCStr("");
    PageSetString(p, 200, s2);
    retrieved = PageGetString(p, 200);
    assert_string_equal(CStringGetPtr(retrieved), "");
    CStringDestroy(s2);
    CStringDestroy(retrieved);

    // 跨页边界测试
    CString *s3 = CStringCreateFromCStr("BoundaryTest");
    PageSetString(p, 1000, s3);
    retrieved = PageGetString(p, 1000);
    assert_string_equal(CStringGetPtr(retrieved), "BoundaryTest");
    CStringDestroy(s3);
    CStringDestroy(retrieved);

    PageDestroy(p);
}

// 测试短整型和长整型
static void test_PageShotLong(void **state) {
    Page *p = PageInit(1024);

    // 短整型测试
    short s_val = 12345;
    PageSetInt(p, 300, s_val);
    short retrieved_short = PageGetShort(p, 300);
    assert_int_equal(retrieved_short, 12345);

    // 长整型测试
    long l_val = 123456789;
    PageSetInt(p, 400, l_val);
    long long retrieved_long = PageGetLong(p, 400);
    assert_int_equal(retrieved_long, 123456789);

    PageDestroy(p);
}

// 测试字节数组
static void test_PageBytes(void **state) {
    Page *p = PageInit(1024);

    uint8_t test_bytes[] = {0x01, 0x02, 0x03, 0x04};
    PageSetBytes(p, 500, test_bytes,sizeof(test_bytes));

    uint8_t *retrieved = PageGetBytes(p, 500);
    for (int i = 0; i < 4; i++) {
        assert_int_equal(retrieved[i], test_bytes[i]);
    }
    PageSetBytesRaw(p, 500, test_bytes, sizeof(test_bytes));
    PageGetBytesRaw(p, 500, test_bytes, sizeof(test_bytes));
    for (int i = 0; i < 4; i++) {
        assert_int_equal(retrieved[i], test_bytes[i]);
    }
    PageDestroy(p);
}

// 测试最大长度计算
static void test_PageMaxLength(void **state) {
    CString *s = CStringCreateFromCStr("Test");
    int max_len = PageMaxLength(s);
    // 假设实现为：字符串长度 + 长度前缀(4字节)
    assert_int_equal(max_len, 4 + 4);  // 4字符 + 4字节长度前缀
    CStringDestroy(s);
}

// 测试错误处理


int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_PageInit),
            cmocka_unit_test(test_PageInt),
            cmocka_unit_test(test_PageString),
            cmocka_unit_test(test_PageShotLong),
            cmocka_unit_test(test_PageBytes),
            cmocka_unit_test(test_PageMaxLength),

    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}