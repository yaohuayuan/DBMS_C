#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include <cmocka.h>

#include "DBError.h"

static DBStatus helper_return_error(DBError *err) {
    DB_RETURN_ERROR(err, DB_ERR_INTERNAL, "DBErrorTest", "helper failed");
}

static DBStatus helper_ok(void) {
    return DB_OK;
}

static DBStatus helper_internal_error(void) {
    return DB_ERR_INTERNAL;
}

static DBStatus helper_try_ok(void) {
    DB_TRY(helper_ok());
    return DB_OK;
}

static DBStatus helper_try_error(void) {
    DB_TRY(helper_internal_error());
    return DB_OK;
}

static void test_init_sets_ok_and_empty_message(void **state) {
    (void)state;

    DBError err;
    DBErrorInit(&err);

    assert_int_equal(err.code, DB_OK);
    assert_null(err.module);
    assert_null(err.file);
    assert_int_equal(err.line, 0);
    assert_string_equal(err.message, "");

    DBErrorInit(NULL);
}

static void test_status_helpers(void **state) {
    (void)state;

    assert_true(DBStatusIsOk(DB_OK));
    assert_false(DBStatusIsOk(DB_ERR_INTERNAL));
    assert_non_null(DBStatusToString(DB_OK));
    assert_true(strlen(DBStatusToString(DB_OK)) > 0);
    assert_non_null(DBStatusToString(DB_ERR_INTERNAL));
    assert_true(strlen(DBStatusToString(DB_ERR_INTERNAL)) > 0);
    assert_non_null(DBStatusToString((DBStatus)9999));
    assert_true(strlen(DBStatusToString((DBStatus)9999)) > 0);
}

static void test_set_stores_error_context(void **state) {
    (void)state;

    DBError err;
    DBErrorInit(&err);

    DBErrorSet(&err, DB_ERR_FILE_OPEN_FAILED, "File", "FileManager.c", 42, "open failed");

    assert_int_equal(err.code, DB_ERR_FILE_OPEN_FAILED);
    assert_string_equal(err.module, "File");
    assert_string_equal(err.file, "FileManager.c");
    assert_int_equal(err.line, 42);
    assert_string_equal(err.message, "open failed");

    DBErrorSet(NULL, DB_ERR_INTERNAL, "Module", "file.c", 1, "ignored");
}

static void test_setf_formats_message(void **state) {
    (void)state;

    DBError err;
    DBErrorInit(&err);

    DBErrorSetf(&err, DB_ERR_INTERNAL, "Core", "core.c", 7, "value=%d name=%s", 12, "abc");

    assert_int_equal(err.code, DB_ERR_INTERNAL);
    assert_string_equal(err.module, "Core");
    assert_string_equal(err.file, "core.c");
    assert_int_equal(err.line, 7);
    assert_string_equal(err.message, "value=12 name=abc");

    DBErrorSetf(&err, DB_ERR_INTERNAL, "Core", "core.c", 8, NULL);
    assert_string_equal(err.message, "");
    DBErrorSetf(NULL, DB_ERR_INTERNAL, "Core", "core.c", 9, "ignored");
}

static void test_clear_resets_error(void **state) {
    (void)state;

    DBError err;
    DBErrorSet(&err, DB_ERR_INTERNAL, "Core", "core.c", 11, "failed");
    DBErrorClear(&err);

    assert_int_equal(err.code, DB_OK);
    assert_null(err.module);
    assert_null(err.file);
    assert_int_equal(err.line, 0);
    assert_string_equal(err.message, "");

    DBErrorClear(NULL);
}

static void test_print_allows_null_inputs(void **state) {
    (void)state;

    DBError err;
    DBErrorSet(&err, DB_ERR_INTERNAL, "Core", "core.c", 13, "failed");

    DBErrorPrint(NULL, NULL);
    DBErrorPrint(&err, NULL);
}

static void test_macros_set_and_propagate_status(void **state) {
    (void)state;

    DBError err;
    DBErrorInit(&err);

    DB_SET_ERROR(&err, DB_ERR_INVALID_ARGUMENT, "Macro", "bad argument");
    assert_int_equal(err.code, DB_ERR_INVALID_ARGUMENT);
    assert_string_equal(err.module, "Macro");
    assert_string_equal(err.message, "bad argument");
    assert_non_null(err.file);
    assert_true(err.line > 0);

    DB_SET_ERRORF(&err, DB_ERR_INTERNAL, "Macro", "id=%d", 99);
    assert_int_equal(err.code, DB_ERR_INTERNAL);
    assert_string_equal(err.message, "id=99");

    assert_int_equal(helper_return_error(&err), DB_ERR_INTERNAL);
    assert_string_equal(err.module, "DBErrorTest");
    assert_string_equal(err.message, "helper failed");
    assert_int_equal(helper_try_ok(), DB_OK);
    assert_int_equal(helper_try_error(), DB_ERR_INTERNAL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_init_sets_ok_and_empty_message),
        cmocka_unit_test(test_status_helpers),
        cmocka_unit_test(test_set_stores_error_context),
        cmocka_unit_test(test_setf_formats_message),
        cmocka_unit_test(test_clear_resets_error),
        cmocka_unit_test(test_print_allows_null_inputs),
        cmocka_unit_test(test_macros_set_and_propagate_status),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
