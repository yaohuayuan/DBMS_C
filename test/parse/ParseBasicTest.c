#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include "cmocka.h"

#include <stdlib.h>

#include "Parser.h"
#include "Schema.h"

static void destroy_parser(Parser *parser) {
    if (parser == NULL) {
        return;
    }
    LexerFree(parser->lexer);
    free(parser);
}

static void assert_cstring_at(CList *list, size_t index, const char *expected) {
    CString *value = (CString *)CListGet(list, index);
    assert_non_null(value);
    assert_string_equal(CStringGetPtr(value), expected);
}

static void assert_single_id_equals_int(Predicate *predicate, const char *field_name, int expected_value) {
    assert_non_null(predicate);
    assert_non_null(predicate->terms);
    assert_int_equal(predicate->terms->size, 1);

    Term *term = (Term *)CListGet(predicate->terms, 0);
    assert_non_null(term);
    assert_int_equal(term->op, OP_EQ);
    assert_non_null(term->lhs);
    assert_non_null(term->rhs);
    assert_string_equal(ExpressionAsFieldName(term->lhs), field_name);

    Constant *constant = ExpressionAsConstant(term->rhs);
    assert_non_null(constant);
    assert_true(constant->isInt);
    assert_int_equal(ConstantAsInt(constant), expected_value);
}

static void test_parse_select_fields_tables_and_predicate(void **state) {
    (void)state;
    Parser *parser = ParserInit("select id, name from student where id = 1");

    QueryData *query = ParserQuery(parser);

    assert_non_null(query);
    assert_non_null(query->fields);
    assert_non_null(query->tables);
    assert_int_equal(query->fields->size, 2);
    assert_int_equal(query->tables->size, 1);
    assert_cstring_at(query->fields, 0, "id");
    assert_cstring_at(query->fields, 1, "name");
    assert_cstring_at(query->tables, 0, "student");
    assert_single_id_equals_int(query->predicate, "id", 1);

    destroy_parser(parser);
}

static void test_parse_insert_table_fields_and_values(void **state) {
    (void)state;
    Parser *parser = ParserInit("insert into student(id, name) values(1, 'wang')");

    CommandData *command = ParserUpdateCmd(parser);

    assert_non_null(command);
    assert_int_equal(command->code, CMD_INSERT_DATA);
    InsertData *insert = command->data.insertData;
    assert_non_null(insert);
    assert_string_equal(CStringGetPtr(insert->tblname), "student");
    assert_int_equal(insert->flds->size, 2);
    assert_int_equal(insert->vals->size, 2);
    assert_cstring_at(insert->flds, 0, "id");
    assert_cstring_at(insert->flds, 1, "name");

    Constant *id = (Constant *)CListGet(insert->vals, 0);
    Constant *name = (Constant *)CListGet(insert->vals, 1);
    assert_non_null(id);
    assert_non_null(name);
    assert_true(id->isInt);
    assert_false(name->isInt);
    assert_int_equal(ConstantAsInt(id), 1);
    assert_string_equal(ConstantAsString(name), "wang");

    destroy_parser(parser);
}

static void test_parse_update_target_value_and_predicate(void **state) {
    (void)state;
    Parser *parser = ParserInit("update student set name = 'newwang' where id = 1");

    CommandData *command = ParserUpdateCmd(parser);

    assert_non_null(command);
    assert_int_equal(command->code, CMD_MODIFY_DATA);
    ModifyData *modify = command->data.modifyData;
    assert_non_null(modify);
    assert_string_equal(CStringGetPtr(modify->tblname), "student");
    assert_string_equal(CStringGetPtr(modify->fldname), "name");

    Constant *new_value = ExpressionAsConstant(modify->newVal);
    assert_non_null(new_value);
    assert_false(new_value->isInt);
    assert_string_equal(ConstantAsString(new_value), "newwang");
    assert_single_id_equals_int(modify->predicate, "id", 1);

    destroy_parser(parser);
}

static void test_parse_delete_table_and_predicate(void **state) {
    (void)state;
    Parser *parser = ParserInit("delete from student where id = 1");

    CommandData *command = ParserUpdateCmd(parser);

    assert_non_null(command);
    assert_int_equal(command->code, CMD_DELETE_DATA);
    DeleteData *delete_data = command->data.deleteData;
    assert_non_null(delete_data);
    assert_string_equal(CStringGetPtr(delete_data->tblname), "student");
    assert_single_id_equals_int(delete_data->predicate, "id", 1);

    destroy_parser(parser);
}

static void test_parse_create_table_schema(void **state) {
    (void)state;
    Parser *parser = ParserInit("create table student(id int, name varchar(20))");
    CString *id = CStringCreateFromCStr("id");
    CString *name = CStringCreateFromCStr("name");

    CommandData *command = ParserUpdateCmd(parser);

    assert_non_null(command);
    assert_int_equal(command->code, CMD_CREATE_TABLE);
    CreateTableData *create_table = command->data.createTableData;
    assert_non_null(create_table);
    assert_string_equal(CStringGetPtr(create_table->tblname), "student");
    assert_true(SchemaHasField(create_table->schema, id));
    assert_true(SchemaHasField(create_table->schema, name));
    assert_int_equal(SchemaType(create_table->schema, id), FILE_INFO_CODE_INTEGER);
    assert_int_equal(SchemaType(create_table->schema, name), FILE_INFO_CODE_VARCHAR);
    assert_int_equal(SchemaLength(create_table->schema, id), 4);
    assert_int_equal(SchemaLength(create_table->schema, name), 20);

    CStringDestroy(id);
    CStringDestroy(name);
    destroy_parser(parser);
}

static void test_parse_create_view_query_data(void **state) {
    (void)state;
    Parser *parser = ParserInit("create view student_view as select id from student where id = 1");

    CommandData *command = ParserUpdateCmd(parser);

    assert_non_null(command);
    assert_int_equal(command->code, CMD_CREATE_VIEW);
    CreateViewData *create_view = command->data.createViewData;
    assert_non_null(create_view);
    assert_string_equal(CStringGetPtr(create_view->viewName), "student_view");
    assert_non_null(create_view->queryData);
    assert_int_equal(create_view->queryData->fields->size, 1);
    assert_int_equal(create_view->queryData->tables->size, 1);
    assert_cstring_at(create_view->queryData->fields, 0, "id");
    assert_cstring_at(create_view->queryData->tables, 0, "student");
    assert_single_id_equals_int(create_view->queryData->predicate, "id", 1);

    destroy_parser(parser);
}

static void test_parse_create_index_identifiers(void **state) {
    (void)state;
    Parser *parser = ParserInit("create index idx_student_id on student(id)");

    CommandData *command = ParserUpdateCmd(parser);

    assert_non_null(command);
    assert_int_equal(command->code, CMD_CREATE_INDEX);
    CreateIndexData *create_index = command->data.createIndexData;
    assert_non_null(create_index);
    assert_string_equal(CStringGetPtr(create_index->idxname), "idx_student_id");
    assert_string_equal(CStringGetPtr(create_index->tblname), "student");
    assert_string_equal(CStringGetPtr(create_index->fldname), "id");

    destroy_parser(parser);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_parse_select_fields_tables_and_predicate),
        cmocka_unit_test(test_parse_insert_table_fields_and_values),
        cmocka_unit_test(test_parse_update_target_value_and_predicate),
        cmocka_unit_test(test_parse_delete_table_and_predicate),
        cmocka_unit_test(test_parse_create_table_schema),
        cmocka_unit_test(test_parse_create_view_query_data),
        cmocka_unit_test(test_parse_create_index_identifiers),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
