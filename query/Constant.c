//
// Created by yaohuayuan on 2024/11/19.
//

#include "Constant.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// 创建新的整数常量
Constant* ConstantInitInt(int ival) {
    Constant *c = (Constant*)malloc(sizeof(Constant));
    c->isInt = true;
    c->value.ival = ival;
    return c;
}

// 创建新的字符串常量
Constant* ConstantInitString(const char *sval) {
    Constant *c = (Constant*)malloc(sizeof(Constant));
    c->isInt = false;
    c->value.sval = strdup(sval); // 使用strdup复制字符串
    return c;
}

// 获取常量的整数值
int ConstantAsInt(Constant *c) {
    if (!c->isInt) {
        fprintf(stderr, "Error: Trying to get integer value from string constant.\n");
        exit(EXIT_FAILURE);
    }
    return c->value.ival;
}

// 获取常量的字符串值
const char* ConstantAsString(Constant *c) {
    if (c->isInt) {
        fprintf(stderr, "Error: Trying to get string value from integer constant.\n");
        exit(EXIT_FAILURE);
    }
    return c->value.sval;
}

// 比较两个常量
int ConstantCompareTo(Constant *c1, Constant *c2) {
    if (c1->isInt && c2->isInt) {
        return c1->value.ival - c2->value.ival;
    } else if (!c1->isInt && !c2->isInt) {
        return strcmp(c1->value.sval, c2->value.sval);
    } else {
        fprintf(stderr, "Error: Comparing different types of constants.\n");
        exit(EXIT_FAILURE);
    }
}

// 检查两个常量是否相等
bool ConstantEquals(Constant *c1, Constant *c2) {
    if (c1->isInt && c2->isInt) {
        return c1->value.ival == c2->value.ival;
    } else if (!c1->isInt && !c2->isInt) {
        return strcmp(c1->value.sval, c2->value.sval) == 0;
    } else {
        return false;
    }
}
static unsigned string_hash(const char *str) {
    unsigned hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) ^ *str++;
    }
    return hash;
}
// 获取常量的哈希码
int ConstantHashCode(Constant *c) {
    if (c->isInt) {
        return c->value.ival;
    } else {
        return (int)string_hash(c->value.sval);
    }
}
char* ConstantItoa(int value) {
    char *buffer = (char*)malloc(12); // 足够大的缓冲区
    sprintf(buffer, "%d", value);
    return buffer;
}
// 将常量转换为字符串
char* ConstantToString(Constant *c) {
    if (c->isInt) {
        return ConstantItoa(c->value.ival); // 使用itoa将整数转换为字符串
    } else {
        return strdup(c->value.sval); // 使用strdup复制字符串
    }
}

// 释放常量资源
void ConstantFree(Constant *c) {
    if (!c->isInt) {
        free(c->value.sval); // 释放字符串内存
    }
    free(c);
}
