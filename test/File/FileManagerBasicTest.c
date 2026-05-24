#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmocka.h>

#include "BlockID.h"
#include "CString.h"
#include "FileManager.h"
#include "Page.h"

#ifdef _WIN32
#include <direct.h>
#include <process.h>
#define RMDIR(path) _rmdir(path)
#define GETPID() _getpid()
#else
#include <unistd.h>
#define RMDIR(path) rmdir(path)
#define GETPID() getpid()
#endif

#define TEST_DB_DIR "file_manager_basic_test_db"
#define TEST_FILE_NAME "page_roundtrip.tbl"
#define APPEND_TEST_DB_DIR "file_manager_append_test_db"
#define APPEND_TEST_FILE_NAME "append_roundtrip.tbl"
#define TEST_BLOCK_SIZE 400

static void cleanup_db_file(const char *directory, const char *filename) {
    char path[256];
    snprintf(path, sizeof(path), "%s/%s", directory, filename);
    remove(path);
#ifdef _WIN32
    snprintf(path, sizeof(path), "%s\\%s", directory, filename);
    remove(path);
#endif
    RMDIR(directory);
}

static void test_page_write_read_int(void **state) {
    (void)state;

    Page *page = PageInit(TEST_BLOCK_SIZE);

    assert_non_null(page);
    PageSetInt(page, 16, 12345);
    assert_int_equal(PageGetInt(page, 16), 12345);

    PageDestroy(page);
}

static void test_page_write_read_string(void **state) {
    (void)state;

    Page *page = PageInit(TEST_BLOCK_SIZE);
    CString *input = CStringCreateFromCStr("hello-page");

    assert_non_null(page);
    assert_non_null(input);
    PageSetString(page, 32, input);

    CString *output = PageGetString(page, 32);
    assert_non_null(output);
    assert_string_equal(CStringGetPtr(output), "hello-page");

    CStringDestroy(output);
    CStringDestroy(input);
    PageDestroy(page);
}

static void test_block_id_create_compare_and_string(void **state) {
    (void)state;

    CString *fileName = CStringCreateFromCStr(TEST_FILE_NAME);
    BlockID *first = BlockIDInit(fileName, 3);
    BlockID *same = BlockIDInit(fileName, 3);
    BlockID *different = BlockIDInit(fileName, 4);

    assert_non_null(first);
    assert_non_null(same);
    assert_non_null(different);
    assert_string_equal(CStringGetPtr(BlockIDGetFileName(first)), TEST_FILE_NAME);
    assert_int_equal(BlockIDGetBlockID(first), 3);
    assert_true(BlockIDEqual(first, same));
    assert_false(BlockIDEqual(first, different));

    CString *text = BlockID2CString(first);
    assert_non_null(text);
    assert_string_equal(CStringGetPtr(text), TEST_FILE_NAME ": 3");

    CStringDestroy(text);
    BlockIDDestroy(different);
    BlockIDDestroy(same);
    BlockIDDestroy(first);
    CStringDestroy(fileName);
}

static void test_file_manager_write_and_read_block(void **state) {
    (void)state;
    char dbDirName[128];
    snprintf(dbDirName, sizeof(dbDirName), "%s_%d", TEST_DB_DIR, GETPID());
    cleanup_db_file(dbDirName, TEST_FILE_NAME);

    CString *dbDir = CStringCreateFromCStr(dbDirName);
    CString *fileName = CStringCreateFromCStr(TEST_FILE_NAME);
    FileManager *fileManager = FileManagerInit(dbDir, TEST_BLOCK_SIZE);
    BlockID *block = BlockIDInit(fileName, 0);
    Page *writePage = PageInit(TEST_BLOCK_SIZE);
    Page *readPage = PageInit(TEST_BLOCK_SIZE);
    CString *text = CStringCreateFromCStr("file-manager");

    assert_non_null(dbDir);
    assert_non_null(fileName);
    assert_non_null(fileManager);
    assert_non_null(block);
    assert_non_null(writePage);
    assert_non_null(readPage);
    assert_non_null(text);

    PageSetInt(writePage, 8, 6789);
    PageSetString(writePage, 40, text);
    FileManagerWrite(fileManager, block, writePage);
    FileManagerRead(fileManager, block, readPage);

    CString *readText = PageGetString(readPage, 40);
    assert_int_equal(PageGetInt(readPage, 8), 6789);
    assert_non_null(readText);
    assert_string_equal(CStringGetPtr(readText), "file-manager");

    CStringDestroy(readText);
    CStringDestroy(text);
    PageDestroy(readPage);
    PageDestroy(writePage);
    BlockIDDestroy(block);
    FileManagerDestroy(fileManager);
    CStringDestroy(fileName);
    CStringDestroy(dbDir);
    cleanup_db_file(dbDirName, TEST_FILE_NAME);
}

static void test_file_manager_append_block(void **state) {
    (void)state;
    char dbDirName[128];
    snprintf(dbDirName, sizeof(dbDirName), "%s_%d", APPEND_TEST_DB_DIR, GETPID());
    cleanup_db_file(dbDirName, APPEND_TEST_FILE_NAME);

    CString *dbDir = CStringCreateFromCStr(dbDirName);
    CString *fileName = CStringCreateFromCStr(APPEND_TEST_FILE_NAME);
    FileManager *fileManager = FileManagerInit(dbDir, TEST_BLOCK_SIZE);

    assert_non_null(fileManager);
    assert_int_equal(FileManagerLength(fileManager, fileName), 0);

    BlockID *first = FileManagerAppend(fileManager, fileName);
    assert_non_null(first);
    assert_int_equal(BlockIDGetBlockID(first), 0);
    assert_int_equal(FileManagerLength(fileManager, fileName), 1);

    BlockID *second = FileManagerAppend(fileManager, fileName);
    assert_non_null(second);
    assert_int_equal(BlockIDGetBlockID(second), 1);
    assert_int_equal(FileManagerLength(fileManager, fileName), 2);

    BlockIDDestroy(second);
    BlockIDDestroy(first);
    FileManagerDestroy(fileManager);
    CStringDestroy(fileName);
    CStringDestroy(dbDir);
    cleanup_db_file(dbDirName, APPEND_TEST_FILE_NAME);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_page_write_read_int),
        cmocka_unit_test(test_page_write_read_string),
        cmocka_unit_test(test_block_id_create_compare_and_string),
        cmocka_unit_test(test_file_manager_write_and_read_block),
        cmocka_unit_test(test_file_manager_append_block),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
