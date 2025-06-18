//
// Created by yaohuayuan on 2025/5/30.
//

#ifndef NEWDBMS_CSTRING_H
#define NEWDBMS_CSTRING_H
#include "string.h"
#include "malloc.h"
#include "CCMath.h"
typedef struct CString{
    char* data;
    int length;
    int capacity;
}CString;
/***
 * @brief 利用C的字符串创建一个CString
 * @param char 字符组
 * @return CString
***/
CString* c_string_init(char *data);
/***
 * @brief 释放CString
 * @param CString字符串
 * @return 无
***/

void c_string_free(CString *c_string);

/***
 * @brief 获取比较两个CString的大小
 * @param CString字符串1 CString字符串2
 * @return 当CString1 > CString2 返回1 当CString1 < CString2 返回-1 当CString1 == CString2 返回0
***/
int c_string_compare(CString *c_string1,CString *c_string2);
/***
 * @brief 在CSString中插入一个字符CString字符串
 * @param 被插入字符串c_string1 插入的字符CString字符串c_string2 插入的位置index
 * @return 删除成功返回1 删除失败返回0
***/
int c_string_insert(CString *c_string1,CString *c_string2,int index);
/***
 * @brief 在CSString中删除一个字符
 * @param CString字符串 删除的位置index
 * @@return 删除成功返回1 删除失败返回0
 */
int c_string_delete(CString *c_string,int index);
 /***
 * @brief 在CSString中末尾追加一个字符串CString
 * @param  被追加字符串CString1 追加的字符Cstring2
 * @@return 删除成功返回1
 */
 int c_string_append(CString *c_string1,CString *c_string2);

/***
 * @brief 创建一个CString的副本
 * @param data 要复制的CString字符串
 * @return 新创建的CString副本
***/
CString* c_string_copy(CString *data);
#endif //NEWDBMS_CSTRING_H
