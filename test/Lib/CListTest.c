#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>

#include "CList.h"

static int g_free_count;
static int g_print_count;
static int g_visit_sum;

static int *new_int(int value) {
    int *ptr = malloc(sizeof(int));
    assert_non_null(ptr);
    *ptr = value;
    return ptr;
}

static bool int_equals(const void *a, const void *b) {
    if (!a || !b) {
        return false;
    }
    return *(const int *)a == *(const int *)b;
}

static void counting_free(void *data) {
    if (data) {
        g_free_count++;
        free(data);
    }
}

static void counting_print(const void *data) {
    assert_non_null(data);
    g_print_count++;
}

static void *copy_int(const void *data) {
    if (!data) {
        return NULL;
    }
    return new_int(*(const int *)data);
}

static bool sum_visitor(void *data, void *context) {
    (void)context;
    if (!data) {
        return true;
    }
    g_visit_sum += *(int *)data;
    return true;
}

static bool stop_after_first_visitor(void *data, void *context) {
    (void)data;
    int *count = context;
    (*count)++;
    return false;
}

static void test_init_sets_empty_list(void **state) {
    (void)state;

    CList *list = CListInit(counting_free, int_equals, counting_print);

    assert_non_null(list);
    assert_null(list->head);
    assert_null(list->tail);
    assert_int_equal(list->size, 0);
    assert_ptr_equal(list->freeFunc, counting_free);
    assert_ptr_equal(list->equalsFunc, int_equals);
    assert_ptr_equal(list->printFunc, counting_print);
    assert_null(list->copyFunc);

    CListFree(list);
}

static void test_append_and_get_store_original_pointers(void **state) {
    (void)state;

    int first = 10;
    int second = 20;
    CList *list = CListInit(NULL, NULL, NULL);

    assert_true(CListAppend(list, &first));
    assert_true(CListAppend(list, &second));

    assert_int_equal(list->size, 2);
    assert_ptr_equal(list->tail->data, &second);
    assert_ptr_equal(CListGet(list, 0), &first);
    assert_ptr_equal(CListGet(list, 1), &second);
    assert_null(CListGet(list, 2));

    CListFree(list);
}

static void test_contains_uses_equals_callback(void **state) {
    (void)state;

    int a = 1;
    int b = 2;
    int same_as_a = 1;
    int missing = 3;
    CList *list = CListInit(NULL, int_equals, NULL);

    assert_true(CListAppend(list, &a));
    assert_true(CListAppend(list, &b));

    assert_true(CListContains(list, &same_as_a));
    assert_false(CListContains(list, &missing));
    assert_false(CListContains(NULL, &same_as_a));
    assert_int_equal(CListIndexOf(list, &same_as_a), 0);
    assert_int_equal(CListIndexOf(list, &missing), -1);

    CListFree(list);
}

static void test_contains_without_equals_is_false(void **state) {
    (void)state;

    int value = 7;
    CList *list = CListInit(NULL, NULL, NULL);

    assert_true(CListAppend(list, &value));
    assert_false(CListContains(list, &value));

    CListFree(list);
}

static void test_remove_by_index_returns_data_without_freeing_it(void **state) {
    (void)state;

    g_free_count = 0;
    int *first = new_int(1);
    int *second = new_int(2);
    CList *list = CListInit(counting_free, int_equals, NULL);

    assert_true(CListAppend(list, first));
    assert_true(CListAppend(list, second));

    int *removed = CListRemoveByIndex(list, 0);
    assert_ptr_equal(removed, first);
    assert_int_equal(*removed, 1);
    assert_int_equal(list->size, 1);
    assert_ptr_equal(list->tail->data, second);
    assert_ptr_equal(CListGet(list, 0), second);
    assert_int_equal(g_free_count, 0);
    assert_null(CListRemoveByIndex(list, 99));
    assert_null(CListRemoveByIndex(NULL, 0));

    free(removed);
    CListFree(list);
    assert_int_equal(g_free_count, 1);
}

static void test_remove_by_value_can_free_removed_data(void **state) {
    (void)state;

    g_free_count = 0;
    int *first = new_int(11);
    int *second = new_int(22);
    int key = 11;
    int missing = 33;
    CList *list = CListInit(counting_free, int_equals, NULL);

    assert_true(CListAppend(list, first));
    assert_true(CListAppend(list, second));

    assert_true(CListRemoveByValue(list, &key));
    assert_int_equal(g_free_count, 1);
    assert_int_equal(list->size, 1);
    assert_ptr_equal(CListGet(list, 0), second);

    assert_false(CListRemoveByValue(list, &missing));
    assert_false(CListRemoveByValue(NULL, &missing));

    CListFree(list);
    assert_int_equal(g_free_count, 2);
}

static void test_add_all_is_shallow_copy(void **state) {
    (void)state;

    int first = 1;
    int second = 2;
    CList *src = CListInit(NULL, NULL, NULL);
    CList *dest = CListInit(NULL, NULL, NULL);

    assert_true(CListAppend(src, &first));
    assert_true(CListAppend(src, &second));
    assert_true(CListAddAll(dest, src));

    assert_int_equal(dest->size, 2);
    assert_ptr_equal(CListGet(dest, 0), &first);
    assert_ptr_equal(CListGet(dest, 1), &second);
    assert_true(CListAppend(src, NULL));
    assert_false(CListAddAll(dest, NULL));
    assert_false(CListAddAll(NULL, src));

    CListFree(dest);
    CListFree(src);
}

static void test_append_copy_uses_copy_callback(void **state) {
    (void)state;

    int value = 42;
    g_free_count = 0;
    CList *list = CListInitWithCopy(counting_free, int_equals, NULL, copy_int);

    assert_true(CListAppendCopy(list, &value));
    int *stored = CListGet(list, 0);

    assert_non_null(stored);
    assert_int_equal(*stored, value);
    assert_ptr_not_equal(stored, &value);
    assert_false(CListAppendCopy(NULL, &value));

    CListFree(list);
    assert_int_equal(g_free_count, 1);
}

static void test_add_all_deep_copies_each_object(void **state) {
    (void)state;

    int first = 5;
    int second = 8;
    g_free_count = 0;
    CList *src = CListInit(NULL, NULL, NULL);
    CList *dest = CListInitWithCopy(counting_free, int_equals, NULL, copy_int);

    assert_true(CListAppend(src, &first));
    assert_true(CListAppend(src, &second));
    assert_true(CListAddAllDeep(dest, src));

    int *copy_first = CListGet(dest, 0);
    int *copy_second = CListGet(dest, 1);
    assert_int_equal(*copy_first, first);
    assert_int_equal(*copy_second, second);
    assert_ptr_not_equal(copy_first, &first);
    assert_ptr_not_equal(copy_second, &second);
    assert_false(CListAddAllDeep(NULL, src));
    assert_false(CListAddAllDeep(dest, NULL));

    CListFree(dest);
    CListFree(src);
    assert_int_equal(g_free_count, 2);
}

static void test_clone_shallow_does_not_own_data(void **state) {
    (void)state;

    int first = 1;
    int second = 2;
    CList *src = CListInitWithCopy(counting_free, int_equals, NULL, copy_int);
    assert_true(CListAppend(src, &first));
    assert_true(CListAppend(src, &second));

    CList *clone = CListCloneShallow(src);
    assert_non_null(clone);
    assert_null(clone->freeFunc);
    assert_int_equal(clone->size, 2);
    assert_ptr_equal(CListGet(clone, 0), &first);
    assert_ptr_equal(CListGet(clone, 1), &second);

    CListFree(clone);
    src->freeFunc = NULL;
    CListFree(src);
}

static void test_clone_deep_owns_copied_data(void **state) {
    (void)state;

    int first = 9;
    int second = 10;
    g_free_count = 0;
    CList *src = CListInitWithCopy(counting_free, int_equals, NULL, copy_int);
    assert_true(CListAppend(src, &first));
    assert_true(CListAppend(src, &second));

    CList *clone = CListCloneDeep(src);
    assert_non_null(clone);
    assert_int_equal(clone->size, 2);
    assert_ptr_not_equal(CListGet(clone, 0), &first);
    assert_ptr_not_equal(CListGet(clone, 1), &second);
    assert_int_equal(*(int *)CListGet(clone, 0), first);
    assert_int_equal(*(int *)CListGet(clone, 1), second);
    assert_null(CListCloneDeep(NULL));

    CListFree(clone);
    src->freeFunc = NULL;
    CListFree(src);
    assert_int_equal(g_free_count, 2);
}

static void test_for_each_and_clear(void **state) {
    (void)state;

    g_visit_sum = 0;
    g_free_count = 0;
    int *first = new_int(3);
    int *second = new_int(4);
    CList *list = CListInit(counting_free, NULL, NULL);

    assert_true(CListAppend(list, first));
    assert_true(CListAppend(list, second));
    assert_true(CListForEach(list, sum_visitor, NULL));
    assert_int_equal(g_visit_sum, 7);

    int visits = 0;
    assert_false(CListForEach(list, stop_after_first_visitor, &visits));
    assert_int_equal(visits, 1);
    assert_false(CListForEach(NULL, sum_visitor, NULL));

    CListClear(list, false);
    assert_true(CListIsEmpty(list));
    assert_null(list->head);
    assert_null(list->tail);
    assert_int_equal(g_free_count, 0);

    free(first);
    free(second);
    CListFree(list);
}

static void test_clear_can_free_owned_data(void **state) {
    (void)state;

    g_free_count = 0;
    CList *list = CListInit(counting_free, NULL, NULL);
    assert_true(CListAppend(list, new_int(1)));
    assert_true(CListAppend(list, new_int(2)));

    CListClear(list, true);
    assert_true(CListIsEmpty(list));
    assert_int_equal(g_free_count, 2);

    CListFree(list);
}

static void test_print_uses_print_callback(void **state) {
    (void)state;

    int a = 1;
    int b = 2;
    g_print_count = 0;
    CList *list = CListInit(NULL, NULL, counting_print);

    assert_true(CListAppend(list, &a));
    assert_true(CListAppend(list, &b));

    CListPrint(list);
    assert_int_equal(g_print_count, 2);

    CListPrint(NULL);
    CListFree(list);
}

static void test_free_allows_null_and_frees_each_data(void **state) {
    (void)state;

    g_free_count = 0;
    CList *list = CListInit(counting_free, NULL, NULL);

    assert_true(CListAppend(list, new_int(1)));
    assert_true(CListAppend(list, new_int(2)));

    CListFree(NULL);
    CListFree(list);
    assert_int_equal(g_free_count, 2);
}

static void test_append_null_list_fails(void **state) {
    (void)state;

    int value = 1;
    assert_false(CListAppend(NULL, &value));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_init_sets_empty_list),
        cmocka_unit_test(test_append_and_get_store_original_pointers),
        cmocka_unit_test(test_contains_uses_equals_callback),
        cmocka_unit_test(test_contains_without_equals_is_false),
        cmocka_unit_test(test_remove_by_index_returns_data_without_freeing_it),
        cmocka_unit_test(test_remove_by_value_can_free_removed_data),
        cmocka_unit_test(test_add_all_is_shallow_copy),
        cmocka_unit_test(test_append_copy_uses_copy_callback),
        cmocka_unit_test(test_add_all_deep_copies_each_object),
        cmocka_unit_test(test_clone_shallow_does_not_own_data),
        cmocka_unit_test(test_clone_deep_owns_copied_data),
        cmocka_unit_test(test_for_each_and_clear),
        cmocka_unit_test(test_clear_can_free_owned_data),
        cmocka_unit_test(test_print_uses_print_callback),
        cmocka_unit_test(test_free_allows_null_and_frees_each_data),
        cmocka_unit_test(test_append_null_list_fails),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
