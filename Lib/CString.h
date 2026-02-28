//
// Created by yaohuayuan on 2025/5/30.
//

#ifndef NEWDBMS_CSTRING_H
#define NEWDBMS_CSTRING_H
#include "string.h"
#include "malloc.h"
#include "CCMath.h"
#include <stdlib.h>
#include <stdbool.h>

// CString 结构体（不透明类型）
typedef struct CString {
    char* data;     // 字符串数据指针
    size_t length;  // 当前字符串长度（不含空终止符）
    size_t capacity; // 分配的内存容量
}CString;
/**
 * 创建一个新的空CString实例。
 *
 * @return 成功时返回新创建的CString指针，失败时返回NULL。
 */
CString* CStringCreate(void);

/**
 * 从C风格字符串创建一个CString实例。
 *
 * @param str 源C风格字符串
 * @return 成功时返回新创建的CString指针，失败时返回NULL。
 */
CString* CStringCreateFromCStr(const char* str);

/**
 * 从现有CString实例创建一个新的副本。
 *
 * @param str 源CString实例
 * @return 成功时返回新创建的CString指针，失败时返回NULL。
 */
CString* CStringCreateFromCString(const CString* str);
/**
 * 从现有CString实例深拷贝一个新的副本。
 *
 * @param str 源CString实例
 * @return 成功时返回新创建的CString指针，失败时返回NULL。
 */
void* CStringCreateFromCStringVoid(void* str);


/**
 * 在CString末尾追加C风格字符串。
 *
 * @param cs 目标CString
 * @param str 要追加的C风格字符串
 */
void CStringAppendCStr(CString* cs, const char* str);

/**
 * 在CString末尾追加另一个CString的内容。
 *
 * @param dest 目标CString（被追加的对象）
 * @param src 源CString（要追加的内容）
 * @return 追加成功返回1，失败返回0
 */
int CStringAppendCString(CString* dest, const CString* src);

/**
 * 在CString末尾追加单个字符。
 *
 * @param cs 目标CString
 * @param c 要追加的字符
 */
void CStringAppendChar(CString* cs, char c);

/**
 * 获取CString内部的C风格字符串指针（只读）。
 *
 * @param cs 目标CString
 * @return 指向内部字符串数据的常量指针
 */
const char* CStringGetPtr(const CString* cs);

/**
 * 获取CString的当前长度（不含终止符）。
 *
 * @param cs 目标CString
 * @return 字符串长度
 */
size_t CStringGetLength(const CString* cs);

/**
 * 获取CString的当前容量（分配的内存大小）。
 *
 * @param cs 目标CString
 * @return 容量（字节数）
 */
size_t CStringGetCapacity(const CString* cs);

/**
 * 清空CString内容，使其成为空字符串。
 *
 * @param cs 目标CString
 */
void CStringClear(CString* cs);

/**
 * 比较两个CString是否相等（内容相同）。
 *
 * @param cs1 第一个CString
 * @param cs2 第二个CString
 * @return 相等返回1，否则返回0
 */
int CStringEqual(const CString* cs1, const CString* cs2);

/**
 * 比较两个CString大小。
 *
 * @param cs1 第一个CString
 * @param cs2 第二个CString
 * @return cs1大返回正数，相等返回0，cs1小于返回负数
 */
int CStringCompare(const CString* cs1, const CString* cs2);
/**
 * 比较两个无返回值CString大小。
 *
 * @param cs1 第一个CString
 * @param cs2 第二个CString
 * @return cs1大返回正数，相等返回0，cs1小于返回负数
 */
int CStringCompareVoid(const void* data1, const void* data2);

/**
 * 在CString中查找子串。
 *
 * @param cs 目标CString
 * @param substr 要查找的子串
 * @return 找到返回子串起始位置索引，未找到返回-1
 */
int CStringFind(const CString* cs, const char* substr);

/**
 * 从CString中提取子串。
 *
 * @param cs 源CString
 * @param start 起始位置索引
 * @param length 要提取的长度
 * @return 新创建的CString包含子串内容
 */
CString* CStringSubstring(const CString* cs, size_t start, size_t length);

/**
 * 将CString转换为小写（原地修改）。
 *
 * @param cs 目标CString
 */
void CStringToLower(CString* cs);

/**
 * 将CString转换为大写（原地修改）。
 *
 * @param cs 目标CString
 */
void CStringToUpper(CString* cs);

/**
 * 在指定位置插入字符串。
 *
 * @param cs 目标CString
 * @param pos 插入位置索引
 * @param str 要插入的字符串
 * @return 插入成功返回1，失败返回0
 */
int CStringInsert(CString* cs, size_t pos, const char* str);

/**
 * 删除CString中的一部分内容。
 *
 * @param cs 目标CString
 * @param start 起始位置索引
 * @param length 要删除的长度
 * @return 删除成功返回1，失败返回0
 */
int CStringRemove(CString* cs, size_t start, size_t length);

/**
 * 替换CString中的子串。
 *
 * @param cs 目标CString
 * @param oldStr 要替换的子串
 * @param newStr 替换后的新子串
 * @return 替换成功返回1，失败返回0
 */
int CStringReplace(CString* cs, const char* oldStr, const char* newStr);

/**
 * 销毁CString实例并释放所有相关内存。
 *
 * @param cs 要销毁的CString实例
 */
void CStringDestroy(CString* cs);

/**
 * 销毁CString实例并释放所有相关内存（void*类型接口，用于通用容器）。
 *
 * @param data 要销毁的CString实例的void*指针
 */
void CStringDestroyVoid(void* data);
#endif //NEWDBMS_CSTRING_H
