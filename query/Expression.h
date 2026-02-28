//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_EXPRESSION_H
#define DBMS_C_EXPRESSION_H
#include "Constant.h"
#include "Schema.h"
#include "Scan.h"
#include "CString.h"
typedef struct Scan  Scan;
/**
 * @brief Expression 结构体，用于表示查询表达式，可以是常量或字段引用。
 */
typedef struct Expression {
    Constant *val;       ///< 常量值，当表达式是常量时有效
    CString *fldname;    ///< 字段名，当表达式是字段引用时有效
} Expression;

/**
 * @brief 创建一个新的常量表达式。
 *
 * @param val 常量值。
 * @return 返回初始化后的 Expression 指针。
 */
Expression* ExpressionInitConstant(Constant *val);

/**
 * @brief 创建一个新的字段名表达式。
 *
 * @param fldname 字段名（C风格字符串）。
 * @return 返回初始化后的 Expression 指针。
 */
Expression* ExpressionInitFieldName(const char *fldname);

/**
 * @brief 创建一个新的字段名表达式（从CString*）。
 *
 * @param fldname 字段名（CString*）。
 * @return 返回初始化后的 Expression 指针。
 */
Expression* ExpressionInitCStringFieldName(CString *fldname);

/**
 * 评估表达式的值。
 *
 * @param expr 指向 Expression 实例的指针。
 * @param s 指向 Scan 实例的指针，用于获取字段值。
 * @return 返回表达式的值，以 Constant 指针形式返回。
 */
Constant* ExpressionEvaluate(Expression *expr, Scan *s);

/**
 * 检查表达式是否是字段引用。
 *
 * @param expr 指向 Expression 实例的指针。
 * @return 如果是字段引用，返回 true；否则返回 false。
 */
bool ExpressionIsFieldName(Expression *expr);

/**
 * 获取表达式的常量值。
 *
 * @param expr 指向 Expression 实例的指针。
 * @return 如果表达式是常量，返回其 Constant 指针；否则返回 NULL。
 */
Constant* ExpressionAsConstant(Expression *expr);

/**
 * 获取表达式的字段名（C风格字符串）。
 *
 * @param expr 指向 Expression 实例的指针。
 * @return 如果是字段引用，返回字段名字符串；否则返回 NULL。
 */
const char* ExpressionAsFieldName(Expression *expr);

/**
 * 获取表达式的字段名（CString*）。
 *
 * @param expr 指向 Expression 实例的指针。
 * @return 如果是字段引用，返回字段名CString*；否则返回 NULL。
 */
CString* ExpressionAsCStringFieldName(Expression *expr);

/**
 * 检查表达式是否适用于给定的模式。
 *
 * @param expr 指向 Expression 实例的指针。
 * @param sch 指向 Schema 实例的指针。
 * @return 如果表达式适用于该模式，返回 true；否则返回 false。
 */
bool ExpressionAppliesTo(Expression *expr, Schema *sch);

/**
 * 将表达式转换为字符串表示。
 *
 * @param expr 指向 Expression 实例的指针。
 * @return 返回表达式的字符串表示。
 */
char* ExpressionToString(Expression *expr);

/**
 * 释放表达式资源。
 *
 * @param expr 指向要释放的 Expression 实例。
 */
void ExpressionFree(Expression *expr);
#endif //DBMS_C_EXPRESSION_H
