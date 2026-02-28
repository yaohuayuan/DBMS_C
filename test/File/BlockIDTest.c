
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include "stdint.h"
#include "BlockID.h"
#include "CString.h"

// 测试 BlockIDInit 和 Getter
static void test_blockid_init_and_getters(void **state) {
    (void)state;

    CString *fileName = CStringCreateFromCStr("testFile.txt");
    BlockID *b = BlockIDInit(fileName, 123);

    assert_non_null(b);
    assert_int_equal(BlockIDGetBlockID(b), 123);
    assert_string_equal(CStringGetPtr(BlockIDGetFileName(b)), "testFile.txt");

    BlockIDDestroy(b);
    CStringDestroy(fileName);
}

// 测试 BlockID2CString
static void test_blockid_to_cstring(void **state) {
    (void)state;

    CString *fileName = CStringCreateFromCStr("test.txt");
    BlockID *b = BlockIDInit(fileName, 456);
    CString *str = BlockID2CString(b);

    assert_non_null(str);
    assert_non_null(CStringGetPtr(str));
    // 简单校验格式包含了 test.txt 和 456
    assert_non_null(strstr(CStringGetPtr(str), "test.txt"));
    assert_non_null(strstr(CStringGetPtr(str), "456"));

    CStringDestroy(str);
    BlockIDDestroy(b);
    CStringDestroy(fileName);
}

// 测试 BlockIDEqual
static void test_blockid_equal(void **state) {
    (void)state;

    BlockID *b1 = BlockIDInit(CStringCreateFromCStr("a.txt"), 1);
    BlockID *b2 = BlockIDInit(CStringCreateFromCStr("a.txt"), 1);
    BlockID *b3 = BlockIDInit(CStringCreateFromCStr("b.txt"), 2);

    assert_true(BlockIDEqual(b1, b2));
    assert_false(BlockIDEqual(b1, b3));

    BlockIDDestroy(b1);
    BlockIDDestroy(b2);
    BlockIDDestroy(b3);
}

// 测试 BlockIDCString2BlockID
static void test_blockid_string_parse(void **state) {
    (void)state;

    BlockID *b = BlockIDInit(CStringCreateFromCStr("parsed.txt"), 789);
    CString *str = BlockID2CString(b);
    BlockID *parsed = BlockIDCString2BlockID(str);

    assert_true(BlockIDEqual(b, parsed));

    BlockIDDestroy(b);
    BlockIDDestroy(parsed);
    CStringDestroy(str);
}

// 主函数：注册所有测试
int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_blockid_init_and_getters),
            cmocka_unit_test(test_blockid_to_cstring),
            cmocka_unit_test(test_blockid_equal),
            cmocka_unit_test(test_blockid_string_parse),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
