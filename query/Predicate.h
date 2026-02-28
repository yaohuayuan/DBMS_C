//
// Created by yaohuayuan on 2024/11/30.
//

#ifndef DBMS_C_PREDICATE_H
#define DBMS_C_PREDICATE_H
#include "List.h"
#include "Term.h"
typedef struct Term Term;
typedef struct Plan Plan;
/**
 * @brief Predicate 结构体，用于表示查询条件，包含多个 Term 对象。
 */
typedef struct Predicate{
    List *terms; ///< Term 对象的列表
}Predicate;

/**
 * @brief 初始化一个新的 Predicate 实例。
 *
 * @param term 初始的 Term 对象。
 * @return 返回初始化后的 Predicate 指针。
 */
Predicate *PredicateInit(Term* term);

/**
 * @brief 将两个谓词连接起来，形成一个新的谓词。
 *
 * @param predicate 第一个谓词。
 * @param predicate1 第二个谓词。
 */
void PredicateConjoinWith(Predicate*predicate,Predicate*predicate1);

/**
 * @brief 检查谓词是否被当前扫描记录满足。
 *
 * @param predicate 指向 Predicate 实例的指针。
 * @param scan 指向 Scan 实例的指针，用于获取当前记录。
 * @return 如果谓词被满足，返回 true；否则返回 false。
 */
bool PredicateIsSatisfied(Predicate*predicate,Scan*scan);

/**
 * @brief 计算谓词的减少因子，用于估计查询计划的效率。
 *
 * @param predicate 指向 Predicate 实例的指针。
 * @param plan 指向 Plan 实例的指针。
 * @return 返回谓词的减少因子。
 */
int PredicateReductionFactor(Predicate*predicate,Plan*plan);

/**
 * @brief 获取谓词中适用于给定模式的选择子谓词。
 *
 * @param predicate 指向 Predicate 实例的指针。
 * @param schema 指向 Schema 实例的指针。
 * @return 返回选择子谓词。
 */
Predicate *PredicateSelectSubPred(Predicate*predicate,Schema*schema);

/**
 * @brief 获取谓词中适用于两个模式连接的子谓词。
 *
 * @param predicate 指向 Predicate 实例的指针。
 * @param schema1 指向第一个 Schema 实例的指针。
 * @param schema2 指向第二个 Schema 实例的指针。
 * @return 返回连接子谓词。
 */
Predicate* PredicateJoinSubPred(Predicate*predicate,Schema *schema1,Schema *schema2);

/**
 * @brief 检查谓词中是否有与指定字段名相等的常量。
 *
 * @param predicate 指向 Predicate 实例的指针。
 * @param fldnam 字段名。
 * @return 如果找到匹配的常量，返回该常量指针；否则返回 NULL。
 */
Constant *PredicateEquatesWithConstant(Predicate*predicate,CString *fldnam);

/**
 * @brief 检查谓词中是否有与指定字段名相等的另一个字段名。
 *
 * @param predicate 指向 Predicate 实例的指针。
 * @param fldnam 字段名。
 * @return 如果找到匹配的字段名，返回该字段名CString*；否则返回 NULL。
 */
CString *PredicateEquatesWithField(Predicate*predicate,CString *fldnam);

/**
 * @brief 将谓词转换为字符串表示。
 *
 * @param predicate 指向 Predicate 实例的指针。
 * @return 返回谓词的字符串表示。
 */
char *PredicateToString(Predicate*predicate);
#endif //DBMS_C_PREDICATE_H
