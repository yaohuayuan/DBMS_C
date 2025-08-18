//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_EXPRESSION_H
#define DBMS_C_EXPRESSION_H
#include "Constant.h"
#include "Schema.h"
#include "Scan.h"
typedef struct Scan  Scan;
typedef struct Expression {
    Constant *val;
    char *fldname;
} Expression;

// 创建新的常量表达式
Expression* ExpressionInitConstant(Constant *val);

// 创建新的字段名表达式
Expression* ExpressionInitFieldName(const char *fldname);

// 评估表达式
Constant* ExpressionEvaluate(Expression *expr, Scan *s);

// 检查表达式是否是字段引用
bool ExpressionIsFieldName(Expression *expr);

// 获取表达式的常量值
Constant* ExpressionAsConstant(Expression *expr);

// 获取表达式的字段名
const char* ExpressionAsFieldName(Expression *expr);

// 检查表达式是否适用于给定的模式
bool ExpressionAppliesTo(Expression *expr, Schema *sch);

// 将表达式转换为字符串
char* ExpressionToString(Expression *expr);

// 释放表达式资源
void ExpressionFree(Expression *expr);
#endif //DBMS_C_EXPRESSION_H
