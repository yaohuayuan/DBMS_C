//
// Created by yaohuayuan on 2024/11/19.
//

#include "Expression.h"
#include "CString.h"
#include <stdlib.h>
#include <stdio.h>  // 包含标准输入输出函数
#include <string.h> // 包含字符串处理函数

// 创建新的常量表达式
Expression* ExpressionInitConstant(Constant *val) {
    Expression *expr = (Expression*)malloc(sizeof(Expression));
    if (expr == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    expr->val = val;
    expr->fldname = NULL;
    return expr;
}

// 创建新的字段名表达式
Expression* ExpressionInitFieldName(const char *fldname) {
    Expression *expr = (Expression*)malloc(sizeof(Expression));
    if (expr == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    expr->val = NULL;
    expr->fldname = CStringCreateFromCStr(fldname); // 使用CStringCreateFromCStr创建CString
    if (expr->fldname == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return expr;
}

// 创建新的字段名表达式（从CString*）
Expression* ExpressionInitCStringFieldName(CString *fldname) {
    Expression *expr = (Expression*)malloc(sizeof(Expression));
    if (expr == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    expr->val = NULL;
    expr->fldname = CStringCreateFromCString(fldname); // 使用CStringCreateFromCString复制CString
    if (expr->fldname == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    return expr;
}

// 评估表达式
Constant* ExpressionEvaluate(Expression *expr, Scan *s) {
    if (expr->val != NULL) {
        return expr->val;
    } else {
        return s->getVal(s, expr->fldname);
    }
}

// 检查表达式是否是字段引用
bool ExpressionIsFieldName(Expression *expr) {
    return expr->fldname != NULL;
}

// 获取表达式的常量值
Constant* ExpressionAsConstant(Expression *expr) {
    return expr->val;
}

// 获取表达式的字段名（C风格字符串）
const char* ExpressionAsFieldName(Expression *expr) {
    return expr->fldname != NULL ? CStringGetPtr(expr->fldname) : NULL;
}

// 获取表达式的字段名（CString*）
CString* ExpressionAsCStringFieldName(Expression *expr) {
    return expr->fldname;
}

// 检查表达式是否适用于给定的模式
bool ExpressionAppliesTo(Expression *expr, Schema *sch) {
    if (expr->val != NULL) {
        return true;
    } else {
        return SchemaHasField(sch, expr->fldname);
    }
}

// 将表达式转换为字符串
char* ExpressionToString(Expression *expr) {
    if (expr->val != NULL) {
        return ConstantToString(expr->val);
    } else {
        return strdup(CStringGetPtr(expr->fldname)); // 使用CStringGetPtr获取C风格字符串
    }
}

// 释放表达式资源
void ExpressionFree(Expression *expr) {
    if (expr->fldname != NULL) {
        CStringDestroy(expr->fldname); // 释放CString内存
    }
    free(expr);
}