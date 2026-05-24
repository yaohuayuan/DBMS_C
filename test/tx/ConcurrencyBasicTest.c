#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>

#include "BlockID.h"
#include "ConcurrencyManager.h"
#include "CString.h"
#include "Error.h"
#include "LockTable.h"

static BlockID *make_block(const char *fileName, int blockNumber) {
    CString *name = CStringCreateFromCStr(fileName);
    BlockID *block = BlockIDInit(name, blockNumber);
    CStringDestroy(name);
    return block;
}

static void destroy_error(Error *error) {
    free(error);
}

static void destroy_lock_table(LockTable *lockTable) {
    if (!lockTable) {
        return;
    }
    if (lockTable->Locks) {
        map_deinit(lockTable->Locks);
        free(lockTable->Locks);
    }
    free(lockTable);
}

static void destroy_concurrency_manager(ConCurrencyManager *manager) {
    if (!manager) {
        return;
    }
    if (manager->mapStr) {
        map_deinit(manager->mapStr);
        free(manager->mapStr);
    }
    free(manager);
}

static const char *local_lock_type(ConCurrencyManager *manager, BlockID *block) {
    CString *key = BlockID2CString(block);
    char **value = map_get(manager->mapStr, CStringGetPtr(key));
    CStringDestroy(key);
    return value == NULL ? NULL : *value;
}

static void test_lock_table_slock_then_unlock(void **state) {
    (void)state;
    LockTable *lockTable = LockTableInit();
    BlockID *block = make_block("concurrency_basic.tbl", 1);
    Error *error = ErrorInit();

    LockTableSLock(lockTable, block, error);

    assert_int_equal(error->errorCode, Error_NULL);
    assert_int_equal(LockTableGetLocalVal(lockTable, block), 1);
    assert_false(LockTableHasXLock(lockTable, block));
    assert_false(LockTableHasSOtherLock(lockTable, block));

    LockTableUnLock(lockTable, block);
    assert_int_equal(LockTableGetLocalVal(lockTable, block), 0);

    destroy_error(error);
    BlockIDDestroy(block);
    destroy_lock_table(lockTable);
}

static void test_lock_table_xlock_then_unlock(void **state) {
    (void)state;
    LockTable *lockTable = LockTableInit();
    BlockID *block = make_block("concurrency_basic.tbl", 2);
    Error *error = ErrorInit();

    LockTableXLock(lockTable, block, error);

    assert_int_equal(error->errorCode, Error_NULL);
    assert_int_equal(LockTableGetLocalVal(lockTable, block), -1);
    assert_true(LockTableHasXLock(lockTable, block));

    LockTableUnLock(lockTable, block);
    assert_int_equal(LockTableGetLocalVal(lockTable, block), 0);

    destroy_error(error);
    BlockIDDestroy(block);
    destroy_lock_table(lockTable);
}

static void test_concurrency_manager_tracks_s_and_x_locks(void **state) {
    (void)state;
    ConCurrencyManager *manager = ConCurrencyManagerInit();
    BlockID *sBlock = make_block("concurrency_manager_basic.tbl", 10);
    BlockID *xBlock = make_block("concurrency_manager_basic.tbl", 11);

    ConCurrencyManagerSLock(manager, sBlock);
    assert_string_equal(local_lock_type(manager, sBlock), "S");
    assert_false(ConCurrencyManagerHasXLock(manager, sBlock));

    ConCurrencyManagerXLock(manager, xBlock);
    assert_string_equal(local_lock_type(manager, xBlock), "X");
    assert_true(ConCurrencyManagerHasXLock(manager, xBlock));

    ConCurrencyManagerRelease(manager);
    assert_int_equal(manager->mapStr->base.nnodes, 0);

    BlockIDDestroy(sBlock);
    BlockIDDestroy(xBlock);
    destroy_concurrency_manager(manager);
}

static void test_concurrency_manager_release_allows_relock(void **state) {
    (void)state;
    BlockID *block = make_block("concurrency_manager_release.tbl", 20);
    ConCurrencyManager *first = ConCurrencyManagerInit();
    ConCurrencyManager *second = ConCurrencyManagerInit();

    ConCurrencyManagerXLock(first, block);
    assert_true(ConCurrencyManagerHasXLock(first, block));
    ConCurrencyManagerRelease(first);
    assert_int_equal(first->mapStr->base.nnodes, 0);

    ConCurrencyManagerSLock(second, block);
    assert_string_equal(local_lock_type(second, block), "S");
    ConCurrencyManagerRelease(second);
    assert_int_equal(second->mapStr->base.nnodes, 0);

    destroy_concurrency_manager(first);
    destroy_concurrency_manager(second);
    BlockIDDestroy(block);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_lock_table_slock_then_unlock),
        cmocka_unit_test(test_lock_table_xlock_then_unlock),
        cmocka_unit_test(test_concurrency_manager_tracks_s_and_x_locks),
        cmocka_unit_test(test_concurrency_manager_release_allows_relock),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
