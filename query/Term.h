//
// Created by yaohuayuan on 2024/11/28.
//

#ifndef DBMS_C_TERM_H
#define DBMS_C_TERM_H
#include "Schema.h"
#include "Scan.h"
#include "Expression.h"
typedef struct Expression Expression;
typedef struct Plan Plan;
/**
 * @brief Term 结构体，用于表示查询条件中的一个术语，由左右两个表达式组成。
 */
typedef struct Term{
    Expression *lhs; ///< 左操作数表达式
    Expression *rhs; ///< 右操作数表达式
}Term;

/**
 * @brief 初始化一个新的 Term 实例。
 *
 * @param lhs 左操作数表达式。
 * @param rhs 右操作数表达式。
 * @return 返回初始化后的 Term 指针。
 */
Term *TermInit(Expression*lhs,Expression*rhs);

/**
 * @brief 检查术语是否被当前扫描记录满足。
 *
 * @param term 指向 Term 实例的指针。
 * @param s 指向 Scan 实例的指针，用于获取当前记录。
 * @return 如果术语被满足，返回 true；否则返回 false。
 */
bool TermIsSatisfied(Term *term,Scan *s);

/**
 * @brief 计算术语的减少因子，用于估计查询计划的效率。
 *
 * @param term 指向 Term 实例的指针。
 * @param plan 指向 Plan 实例的指针。
 * @return 返回术语的减少因子。
 */
int TermReductionFactor(Term*term,Plan*plan);

/**
 * @brief 检查术语是否是字段等于常量的形式。
 *
 * @param term 指向 Term 实例的指针。
 * @param fldname 字段名。
 * @return 如果术语是指定字段等于某个常量，返回该常量指针；否则返回 NULL。
 */
Constant *TermEquatesWithConstant(Term*term,CString *fldname);

/**
 * @brief 检查术语是否是字段等于另一个字段的形式。
 *
 * @param term 指向 Term 实例的指针。
 * @param fldname 字段名。
 * @return 如果术语是指定字段等于另一个字段，返回另一个字段名；否则返回 NULL。
 */
CString *TermEquatesWithField(Term*term,CString *fldname);

/**
 * @brief 检查术语是否适用于给定的模式。
 *
 * @param term 指向 Term 实例的指针。
 * @param schema 指向 Schema 实例的指针。
 * @return 如果术语适用于该模式，返回 true；否则返回 false。
 */
bool TermAppliesTo(Term*term,Schema*schema);

/**
 * @brief 将术语转换为字符串表示。
 *
 * @param term 指向 Term 实例的指针。
 * @return 返回术语的字符串表示。
 */
char *TermToString(Term*term);

/**
 * @brief 检查两个术语是否相等。
 *
 * @param term1 指向第一个 Term 实例的指针。
 * @param term2 指向第二个 Term 实例的指针。
 * @return 如果两个术语相等，返回 true；否则返回 false。
 */
bool TermEquals(Term *term1, Term *term2);

/**
 * @brief 释放术语资源。
 *
 * @param term 指向要释放的 Term 实例的指针。
 */
void TermFree(Term *term);
#endif //DBMS_C_TERM_H
