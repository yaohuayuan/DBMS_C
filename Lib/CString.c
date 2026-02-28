//
// Created by yaohuayuan on 2025/5/30.
//
#include "CString.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>



/**
 * 确保CString有足够的容量容纳新数据。
 *
 * @param cs 目标CString
 * @param needed 需要的最小容量（包括终止符）
 * @return 成功返回true，失败返回false
 */
static bool ensureCapacity(CString* cs, size_t needed) {
    if (cs == NULL)
        return false;

    if (needed < cs->capacity) return true;

    // 计算新容量（1.5倍增长因子）
    size_t newCapacity = cs->capacity * 3 / 2;
    if (newCapacity < needed) newCapacity = needed + 16; // 保证足够

    char* newData = (char*)realloc(cs->data, newCapacity);
    if (!newData) return false;

    cs->data = newData;
    cs->capacity = newCapacity;
    return true;
}

CString* CStringCreate(void) {
    CString* cs = (CString*)malloc(sizeof(CString));
    if (!cs) return NULL;

    cs->capacity = 16; // 初始容量
    cs->data = (char*)malloc(cs->capacity);
    if (!cs->data) {
        free(cs);
        return NULL;
    }

    cs->data[0] = '\0';
    cs->length = 0;
    return cs;
}

CString* CStringCreateFromCStr(const char* str) {
    if (!str) return NULL;

    size_t len = strlen(str);
    CString* cs = (CString*)malloc(sizeof(CString));
    if (!cs) return NULL;

    cs->capacity = len + 1; // 至少需要len+1的空间
    if (cs->capacity < 16) cs->capacity = 16; // 最小容量

    cs->data = (char*)malloc(cs->capacity);
    if (!cs->data) {
        free(cs);
        return NULL;
    }

    strncpy(cs->data, str, len);
    cs->data[len] = '\0';
    cs->length = len;
    return cs;
}

CString* CStringCreateFromCString(const CString* src) {
    if (!src) return NULL;
    return CStringCreateFromCStr(src->data);
}

void CStringAppendCStr(CString* cs, const char* str) {
    if (!cs || !str) return;

    size_t appendLen = strlen(str);
    size_t newLen = cs->length + appendLen;

    if (!ensureCapacity(cs, newLen + 1)) return;

    strcat(cs->data + cs->length, str);
    cs->length = newLen;
}

int CStringAppendCString(CString* dest, const CString* src) {
    if (!dest || !src) return 0;
    CStringAppendCStr(dest, src->data);
    return 1;
}

void CStringAppendChar(CString* cs, char c) {
    if (!cs) return;

    if (!ensureCapacity(cs, cs->length + 2)) return;

    cs->data[cs->length] = c;
    cs->length++;
    cs->data[cs->length] = '\0';
}

const char* CStringGetPtr(const CString* cs) {
    return cs ? cs->data : NULL;
}

size_t CStringGetLength(const CString* cs) {
    return cs ? cs->length : 0;
}

size_t CStringGetCapacity(const CString* cs) {
    return cs ? cs->capacity : 0;
}

void CStringClear(CString* cs) {
    if (!cs) return;
    cs->data[0] = '\0';
    cs->length = 0;
}
int CStringEqual(const CString* cs1, const CString* cs2) {
    if (!cs1 || !cs2) return 0;
    if (cs1->length != cs2->length) return 0;
    return strcmp(cs1->data, cs2->data) == 0 ? 1 : 0;
}
int CStringCompare(const CString* cs1, const CString* cs2) {
    if (!cs1 || !cs2) {
        // 处理NULL指针情况
        if (!cs1 && !cs2) return 0;
        return !cs1 ? -1 : 1;
    }

    // 优化：先比较长度
    if (cs1->length != cs2->length) {
        // 如果长度不同，但共同部分相同，则较长的字符串较大
        int min_len = cs1->length < cs2->length ? cs1->length : cs2->length;
        int cmp = memcmp(cs1->data, cs2->data, min_len);
        return cmp != 0 ? cmp : (int)(cs1->length - cs2->length);
    }

    // 长度相同，直接比较整个内容
    return memcmp(cs1->data, cs2->data, cs1->length);
}
int CStringFind(const CString* cs, const char* substr) {
    if (!cs || !substr) return -1;

    char* pos = strstr(cs->data, substr);
    if (!pos) return -1;

    return (int)(pos - cs->data);
}

CString* CStringSubstring(const CString* cs, size_t start, size_t length) {
    if (!cs || start >= cs->length) {
        // 返回空字符串而不是NULL
        CString* empty = CStringCreate();
        return empty;
    }

    // 计算实际可提取的长度
    if (start + length > cs->length) {
        length = cs->length - start;
    }

    // 创建新的CString
    CString* newStr = CStringCreate();
    if (!newStr) return NULL;

    // 确保有足够容量
    if (!ensureCapacity(newStr, length + 1)) {
        CStringDestroy(newStr);
        return NULL;
    }

    // 复制子串内容
    strncpy(newStr->data, cs->data + start, length);
    newStr->data[length] = '\0';
    newStr->length = length;

    return newStr;
}

void CStringToLower(CString* cs) {
    if (!cs) return;

    for (size_t i = 0; i < cs->length; i++) {
        cs->data[i] = (char)tolower((unsigned char)cs->data[i]);
    }
}

void CStringToUpper(CString* cs) {
    if (!cs) return;

    for (size_t i = 0; i < cs->length; i++) {
        cs->data[i] = (char)toupper((unsigned char)cs->data[i]);
    }
}

int CStringInsert(CString* cs, size_t pos, const char* str) {
    if (!cs || !str || pos > cs->length) return 0;

    size_t insertLen = strlen(str);
    size_t newLen = cs->length + insertLen;

    // 确保有足够容量
    if (!ensureCapacity(cs, newLen + 1)) return 0;

    // 移动原字符串的尾部
    memmove(cs->data + pos + insertLen,
            cs->data + pos,
            cs->length - pos + 1); // +1 包含终止符

    // 插入新内容
    strncpy(cs->data + pos, str, insertLen);
    cs->length = newLen;

    return 1;
}

int CStringRemove(CString* cs, size_t start, size_t length) {
    if (!cs || start >= cs->length) return 0;

    // 计算实际可删除的长度
    if (start + length > cs->length) {
        length = cs->length - start;
    }

    // 移动字符串尾部覆盖删除部分
    memmove(cs->data + start,
            cs->data + start + length,
            cs->length - start - length + 1); // +1 包含终止符

    cs->length -= length;
    return 1;
}

int CStringReplace(CString* cs, const char* oldStr, const char* newStr) {
    if (!cs || !oldStr || !newStr) return 0;

    int pos = CStringFind(cs, oldStr);
    if (pos == -1) return 0; // 没找到

    size_t oldLen = strlen(oldStr);
    size_t newLen = strlen(newStr);

    // 如果新字符串比旧字符串长，需要扩展空间
    if (newLen > oldLen) {
        size_t newSize = cs->length - oldLen + newLen;
        if (!ensureCapacity(cs, newSize + 1)) return 0;
    }

    // 替换操作
    CStringRemove(cs, (size_t)pos, oldLen);
    CStringInsert(cs, (size_t)pos, newStr);

    return 1;
}

void CStringDestroy(CString* cs) {
    if (!cs) return;
    if (cs->data) free(cs->data);
    free(cs);
}
void* CStringCreateFromCStringVoid(void* str) {
    CString *cString = (CString *) str;
    CString *result = CStringCreateFromCString(cString);
    return result;
}
int CStringCompareVoid(const void* data1, const void* data2){
    CString *cs1 = (CString*)data1;
    CString *cs2 = (CString*)data2;
    return CStringCompare(cs1,cs2);
}
void CStringDestroyVoid(void* data) {
    CString *cs = (CString*)data;
    if (!cs) return;
    if (cs->data) free(cs->data);
    free(cs);
}