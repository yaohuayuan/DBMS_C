//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_CONSTANT_H
#define DBMS_C_CONSTANT_H

#include <stdbool.h>

typedef struct {
    bool isInt;
    union {
        int ival;
        char *sval;
    } value;
} Constant;

// 创建新的整数常量
Constant* ConstantInitInt(int ival);

// 创建新的字符串常量
Constant* ConstantInitString(const char *sval);

// 获取常量的整数值
int ConstantAsInt(Constant *c);

// 获取常量的字符串值
const char* ConstantAsString(Constant *c);

// 比较两个常量
int ConstantCompareTo(Constant *c1, Constant *c2);

// 检查两个常量是否相等
bool ConstantEquals(Constant *c1, Constant *c2);

// 获取常量的哈希码
int ConstantHashCode(Constant *c);

// 将常量转换为字符串
char* ConstantToString(Constant *c);

// 释放常量资源
void ConstantFree(Constant *c);


#endif //DBMS_C_CONSTANT_H
