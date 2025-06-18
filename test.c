//
// Created by yaohuayuan on 2025/5/30.
//
#include "CString.h"
#include "BlockID.h"
#include "stdio.h"
void test_cstring() {
    printf("=== CString test ===\n");
    // 初始化
    CString *str1 = c_string_init("Hello");
    CString *str2 = c_string_init("World");

    printf("Initial strings:\n");
    printf("str1: %s, str2: %s\n", str1->data, str2->data);

    // 比较
    printf("\nTesting comparison:\n");
    int cmp_result = c_string_compare(str1, str2);
    printf("Compare str1 and str2: %d (Expected < 0 for different strings)\n", cmp_result);

    // 追加操作
    printf("\nTesting append:\n");
    c_string_append(str1, str2);
    printf("After append str2 to str1: %s (Expected: HelloWorld)\n", str1->data);

    // 插入操作
    printf("\nTesting insert:\n");
    CString *str3 = c_string_init("Beautiful");
    c_string_insert(str1, str3, 5);  // Insert at position 5
    printf("After insert str3 into str1 at index 5: %s (Expected: HelloBeautifulWorld)\n", str1->data);

    // 删除操作
    printf("\nTesting delete:\n");
    c_string_delete(str1, 5);  // Delete the character at index 5 (B)
    printf("After delete at index 5: %s (Expected: HelloeautifulWorld)\n", str1->data);

    // 清理资源
    c_string_free(str1);
    c_string_free(str2);
    c_string_free(str3);
}

void test_block_id() {
    printf("=== BlockID test ===\n");
    BlockID *blockId = block_id_init(c_string_init("test"), 1);
    printf("BlockID: %s, %d\n", blockId->fileName->data, blockId->BlockID);
    CString *cString = blockId_to_CString(blockId);
    printf("CString: %s, %d\n", cString->data, cString->length);
}
int main() {
    //test_cstring();
    test_block_id();
    return 0;
}