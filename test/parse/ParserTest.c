#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"
#include "Parser.h"

/**
 * 测试 1: 解析基础 SELECT 语句
 */
static void test_parser_query_basic(void **state) {
    const char *sql = "SELECT id, name FROM users WHERE age = 20";
    Parser *p = ParserInit(sql);

    QueryData *qd = ParserQuery(p);
    assert_non_null(qd);

    // 1. 验证字段列表 (SELECT id, name)
    // 假设 List 存储的是 CString
    assert_int_equal(qd->fields->size, 2);

    // 2. 验证表列表 (FROM users)
    assert_int_equal(qd->tables->size, 1);

    // 3. 验证谓词 (WHERE age = 20)
    // 这里需要根据你的 Predicate/Term 结构来进一步 assert
    assert_non_null(qd->predicate);

    // 清理 (注意：你需要实现对应的 Deep Free 函数)
    // QueryDataFree(qd);
    // ParserFree(p);
}

/**
 * 测试 2: 解析 INSERT 语句
 */
static void test_parser_insert(void **state) {
    const char *sql = "INSERT INTO students (id, score) VALUES (1, 95)";
    Parser *p = ParserInit(sql);

    CommandData *cmd = ParserInsert(p);
    assert_int_equal(cmd->code, CMD_INSERT_DATA);

    InsertData *id = cmd->data.insertData;
    // 验证表名
    assert_string_equal(CStringGetPtr(id->tblname), "students");
    // 验证字段数与值数
    assert_int_equal(cmd->data.insertData->flds->size, 2);
    assert_int_equal(cmd->data.insertData->vals->size, 2);
}

/**
 * 测试 3: 解析带 AND 的复杂 WHERE 子句
 */
static void test_parser_complex_predicate(void **state) {
    const char *sql = "age >= 18 AND status = 'active'";
    Parser *p = ParserInit(sql);

    // 直接测试 Predicate 解析分支
    Predicate *pred = ParserPredicate(p);
    assert_non_null(pred);

    // 验证是否成功解析了两个 Term (因为有 AND)
    // 根据你的 PredicateInit 和 ConjoinWith 逻辑，
    // 这里通常会形成一个 Term 列表
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_parser_query_basic),
        cmocka_unit_test(test_parser_insert),
        cmocka_unit_test(test_parser_complex_predicate),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}