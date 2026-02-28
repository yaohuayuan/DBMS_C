//
// Created by yaohuayuan on 2024/11/30.
//

#ifndef DBMS_C_MODIFYDATA_H
#define DBMS_C_MODIFYDATA_H
#include "Expression.h"
#include "Predicate.h"
/**
 * @brief ModifyData 结构体定义，用于存储修改操作的数据。
 */
typedef struct ModifyData{
    CString *tblname;  ///< 要修改记录的表名称
    CString *fldname;  ///< 要修改的字段名称
    Expression *newVal; ///< 新值的表达式
    Predicate *predicate; ///< 修改条件，只有满足条件的记录才会被修改
}ModifyData;

/**
 * @brief 初始化一个新的 ModifyData 实例。
 *
 * @param tblname 要修改记录的表名称。
 * @param fldname 要修改的字段名称。
 * @param newVal 新值的表达式。
 * @param predicate 修改条件，只有满足条件的记录才会被修改。
 * @return 返回初始化后的 ModifyData 指针。
 */
ModifyData *ModifyDataInit(CString*tblname,CString *fldname,Expression*newVal,Predicate*predicate);
#endif //DBMS_C_MODIFYDATA_H
