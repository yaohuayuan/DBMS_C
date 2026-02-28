//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_CONSTANT_H
#define DBMS_C_CONSTANT_H

#include <stdbool.h>
#include "CString.h"

/**
 * @brief Constant 结构体，用于表示数据库中的常量值，可以是整数或字符串。
 */
typedef struct Constant{
    bool isInt; ///< 标记常量是否为整数类型
    union {
        int ival;     ///< 整数值
        CString *sval; ///< 字符串值
    } value;     ///< 常量的值
} Constant;

/**
 * @brief 创建一个新的整数常量。
 *
 * @param ival 整数值。
 * @return 返回初始化后的 Constant 指针。
 */
Constant* ConstantInitInt(int ival);

/**
 * @brief 创建一个新的字符串常量。
 *
 * @param sval 字符串值。
 * @return 返回初始化后的 Constant 指针。
 */
Constant* ConstantInitString(const char *sval);

/**
 * @brief 创建一个新的字符串常量（从CString*）。
 *
 * @param sval CString*字符串值。
 * @return 返回初始化后的 Constant 指针。
 */
Constant* ConstantInitCString(CString *sval);

/**
 * @brief 获取常量的整数值。
 *
 * @param c 指向 Constant 实例的指针。
 * @return 返回常量的整数值。
 */
int ConstantAsInt(Constant *c);

/**
 * @brief 获取常量的字符串值。
 *
 * @param c 指向 Constant 实例的指针。
 * @return 返回常量的字符串值。
 */
const char* ConstantAsString(Constant *c);

/**
 * @brief 获取常量的CString*字符串值。
 *
 * @param c 指向 Constant 实例的指针。
 * @return 返回常量的CString*字符串值。
 */
CString* ConstantAsCString(Constant *c);

/**
 * @brief 比较两个常量的大小。
 *
 * @param c1 指向第一个 Constant 实例的指针。
 * @param c2 指向第二个 Constant 实例的指针。
 * @return 如果 c1 < c2，返回负数；如果 c1 > c2，返回正数；否则返回 0。
 */
int ConstantCompareTo(Constant *c1, Constant *c2);

/**
 * @brief 检查两个常量是否相等。
 *
 * @param c1 指向第一个 Constant 实例的指针。
 * @param c2 指向第二个 Constant 实例的指针。
 * @return 如果两个常量相等，返回 true；否则返回 false。
 */
bool ConstantEquals(Constant *c1, Constant *c2);

/**
 * @brief 获取常量的哈希码。
 *
 * @param c 指向 Constant 实例的指针。
 * @return 返回常量的哈希码。
 */
int ConstantHashCode(Constant *c);

/**
 * @brief 将常量转换为字符串表示。
 *
 * @param c 指向 Constant 实例的指针。
 * @return 返回常量的字符串表示。
 */
char* ConstantToString(Constant *c);

/**
 * @brief 释放常量资源。
 *
 * @param c 指向要释放的 Constant 实例的指针。
 */
void ConstantFree(Constant *c);


#endif //DBMS_C_CONSTANT_H
