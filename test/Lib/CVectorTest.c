#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "CVector.h"
#include <string.h>
#include <stdlib.h>

/* ================= 测试用结构体 ================= */

typedef struct {
    int id;
    char name[32];
} TestObj;

/* ================= 回调函数 ================= */

static int test_cmp(const void* a, const void* b) {
    const TestObj* x = a;
    const TestObj* y = b;
    return x->id - y->id;
}

static void test_copy(void* dest, const void* src) {
    memcpy(dest, src, sizeof(TestObj));
}

static int destroy_count = 0;

static void test_destroy(void* data) {
    destroy_count++;
    (void)data;
}

/* ================= 测试用例 ================= */

static void test_init_and_destroy(void **state) {
    (void)state;

    CVector* vec = CVectorInit(sizeof(TestObj),
                               test_destroy,
                               test_cmp,
                               test_copy);

    assert_non_null(vec);
    assert_int_equal(vec->size, 0);
    assert_true(vec->capacity >= VECTOR_INIT_CAPACITY);

    CVectorDestroy(vec);
}

static void test_push_back_and_at(void **state) {
    (void)state;

    CVector* vec = CVectorInit(sizeof(TestObj),
                               NULL,
                               test_cmp,
                               test_copy);

    TestObj a = {1, "Alice"};
    TestObj b = {2, "Bob"};

    CVectorPushBack(vec, &a);
    CVectorPushBack(vec, &b);

    assert_int_equal(vec->size, 2);

    TestObj* p0 = CVectorAt(vec, 0);
    TestObj* p1 = CVectorAt(vec, 1);

    assert_int_equal(p0->id, 1);
    assert_string_equal(p0->name, "Alice");

    assert_int_equal(p1->id, 2);
    assert_string_equal(p1->name, "Bob");

    CVectorDestroy(vec);
}

static void test_insert(void **state) {
    (void)state;

    CVector* vec = CVectorInit(sizeof(TestObj),
                               NULL,
                               test_cmp,
                               test_copy);

    TestObj a = {1, "A"};
    TestObj b = {3, "C"};
    TestObj c = {2, "B"};

    CVectorPushBack(vec, &a);
    CVectorPushBack(vec, &b);

    CVectorInsert(vec, 1, &c); // 插入中间

    assert_int_equal(vec->size, 3);

    assert_int_equal(((TestObj*)CVectorAt(vec, 0))->id, 1);
    assert_int_equal(((TestObj*)CVectorAt(vec, 1))->id, 2);
    assert_int_equal(((TestObj*)CVectorAt(vec, 2))->id, 3);

    CVectorDestroy(vec);
}

static void test_erase(void **state) {
    (void)state;

    destroy_count = 0;

    CVector* vec = CVectorInit(sizeof(TestObj),
                               test_destroy,
                               test_cmp,
                               test_copy);

    TestObj a = {1, "A"};
    TestObj b = {2, "B"};
    TestObj c = {3, "C"};

    CVectorPushBack(vec, &a);
    CVectorPushBack(vec, &b);
    CVectorPushBack(vec, &c);

    CVectorErase(vec, 1); // 删除 b

    assert_int_equal(vec->size, 2);
    assert_int_equal(((TestObj*)CVectorAt(vec, 1))->id, 3);
    assert_int_equal(destroy_count, 1);

    CVectorDestroy(vec);
}

static void test_find(void **state) {
    (void)state;

    CVector* vec = CVectorInit(sizeof(TestObj),
                               NULL,
                               test_cmp,
                               test_copy);

    TestObj a = {1, "A"};
    TestObj b = {2, "B"};

    CVectorPushBack(vec, &a);
    CVectorPushBack(vec, &b);

    size_t idx = CVectorFind(vec, &b);
    assert_int_equal(idx, 1);

    TestObj x = {99, "X"};
    assert_int_equal(CVectorFind(vec, &x), SIZE_MAX);

    CVectorDestroy(vec);
}

static void test_pop_back(void **state) {
    (void)state;

    destroy_count = 0;

    CVector* vec = CVectorInit(sizeof(TestObj),
                               test_destroy,
                               test_cmp,
                               test_copy);

    TestObj a = {1, "A"};
    TestObj b = {2, "B"};

    CVectorPushBack(vec, &a);
    CVectorPushBack(vec, &b);

    CVectorPopBack(vec);

    assert_int_equal(vec->size, 1);
    assert_int_equal(destroy_count, 1);

    CVectorDestroy(vec);
}

/* ================= main ================= */

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_init_and_destroy),
        cmocka_unit_test(test_push_back_and_at),
        cmocka_unit_test(test_insert),
        cmocka_unit_test(test_erase),
        cmocka_unit_test(test_find),
        cmocka_unit_test(test_pop_back),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
