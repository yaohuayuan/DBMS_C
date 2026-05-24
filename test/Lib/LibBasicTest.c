#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

#include "ByteBuffer.h"
#include "CList.h"
#include "CMap.h"
#include "CString.h"
#include "CVector.h"
#include "Error.h"
#include "StreamTokenizer.h"

static int int_compare(const void *a, const void *b) {
    int left = *(const int *)a;
    int right = *(const int *)b;
    return (left > right) - (left < right);
}

static bool int_equals(const void *a, const void *b) {
    return a && b && *(const int *)a == *(const int *)b;
}

static void assert_token_text(StreamTokenizer *tokenizer, const char *expected) {
    assert_non_null(tokenizer);
    assert_non_null(expected);
    assert_int_equal(StreamTokenizerGetLength(tokenizer), (int)strlen(expected));
    assert_int_equal(strncmp(StreamTokenizerGetText(tokenizer), expected, strlen(expected)), 0);
}

static void test_cstring_create_append_and_compare(void **state) {
    (void)state;

    CString *value = CStringCreateFromCStr("DBMS");
    CString *expected = CStringCreateFromCStr("DBMS_C");

    assert_non_null(value);
    assert_non_null(expected);
    CStringAppendCStr(value, "_C");

    assert_string_equal(CStringGetPtr(value), "DBMS_C");
    assert_int_equal(CStringGetLength(value), 6);
    assert_true(CStringEqual(value, expected));

    CStringDestroy(expected);
    CStringDestroy(value);
}

static void test_cvector_push_and_read_ints(void **state) {
    (void)state;

    CVector *vector = CVectorInit(sizeof(int), NULL, int_compare, NULL);
    int first = 10;
    int second = 20;
    int replacement = 30;

    assert_non_null(vector);
    CVectorPushBack(vector, &first);
    CVectorPushBack(vector, &second);

    assert_int_equal(vector->size, 2);
    assert_int_equal(*(int *)CVectorAt(vector, 0), 10);
    assert_int_equal(*(int *)CVectorAt(vector, 1), 20);
    assert_int_equal(CVectorFind(vector, &second), 1);

    CVectorSet(vector, 1, &replacement);
    assert_int_equal(*(int *)CVectorAt(vector, 1), 30);

    CVectorDestroy(vector);
}

static void test_clist_append_get_and_contains(void **state) {
    (void)state;

    CList *list = CListInit(NULL, int_equals, NULL);
    int first = 1;
    int second = 2;
    int same_as_second = 2;

    assert_non_null(list);
    assert_true(CListAppend(list, &first));
    assert_true(CListAppend(list, &second));

    assert_int_equal(list->size, 2);
    assert_ptr_equal(CListGet(list, 0), &first);
    assert_true(CListContains(list, &same_as_second));

    CListFree(list);
}

static void test_cmap_insert_find_and_update(void **state) {
    (void)state;

    CMap map;
    int key = 7;
    int value = 70;
    int updated = 700;

    assert_int_equal(CMapInit(&map, sizeof(int), sizeof(int), int_compare, free, free, NULL, NULL), 1);
    assert_int_equal(CMapInsert(&map, &key, &value), 1);

    int *found = CMapFind(&map, &key);
    assert_non_null(found);
    assert_int_equal(*found, 70);

    assert_int_equal(CMapPut(&map, &key, &updated), 1);
    found = CMapFind(&map, &key);
    assert_non_null(found);
    assert_int_equal(*found, 700);

    CMapDestroy(&map);
}

static void test_bytebuffer_put_flip_and_get_int(void **state) {
    (void)state;

    ByteBuffer *buffer = bufferAllocate(16);
    int32_t output = 0;

    assert_non_null(buffer);
    assert_int_equal(bufferPutInt(buffer, 123456), BYTEBUFFER_OK);
    assert_int_equal(buffer->position, 4);

    bufferFlip(buffer);
    assert_int_equal(buffer->position, 0);
    assert_int_equal(buffer->limit, 4);
    assert_int_equal(bufferGetInt(buffer, &output), BYTEBUFFER_OK);
    assert_int_equal(output, 123456);

    bufferFree(buffer);
}

static void test_stream_tokenizer_splits_simple_sql(void **state) {
    (void)state;

    StreamTokenizer *tokenizer = StreamTokenizerInit("select id from student where id = 1");

    assert_non_null(tokenizer);
    assert_int_equal(StreamTokenizerNext(tokenizer), STT_WORD);
    assert_token_text(tokenizer, "select");

    assert_int_equal(StreamTokenizerNext(tokenizer), STT_WORD);
    assert_token_text(tokenizer, "id");

    assert_int_equal(StreamTokenizerNext(tokenizer), STT_WORD);
    assert_token_text(tokenizer, "from");

    assert_int_equal(StreamTokenizerNext(tokenizer), STT_WORD);
    assert_token_text(tokenizer, "student");

    assert_int_equal(StreamTokenizerNext(tokenizer), STT_WORD);
    assert_token_text(tokenizer, "where");

    assert_int_equal(StreamTokenizerNext(tokenizer), STT_WORD);
    assert_token_text(tokenizer, "id");

    assert_int_equal(StreamTokenizerNext(tokenizer), STT_DELIM);
    assert_token_text(tokenizer, "=");

    assert_int_equal(StreamTokenizerNext(tokenizer), STT_NUMBER);
    assert_int_equal(StreamTokenizerGetInt(tokenizer), 1);

    StreamTokenizerFree(tokenizer);
}

static void test_legacy_error_init_defaults(void **state) {
    (void)state;

    Error *error = ErrorInit();

    assert_non_null(error);
    assert_int_equal(error->errorCode, Error_NULL);
    assert_null(error->reason);

    free(error);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_cstring_create_append_and_compare),
        cmocka_unit_test(test_cvector_push_and_read_ints),
        cmocka_unit_test(test_clist_append_get_and_contains),
        cmocka_unit_test(test_cmap_insert_find_and_update),
        cmocka_unit_test(test_bytebuffer_put_flip_and_get_int),
        cmocka_unit_test(test_stream_tokenizer_splits_simple_sql),
        cmocka_unit_test(test_legacy_error_init_defaults),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
