//
// Created by yaohuayuan on 2024/11/28.
//

#ifndef DBMS_C_INSERTDATA_H
#define DBMS_C_INSERTDATA_H
#include "List.h"
/**
 * @brief InsertData 结构体定义，用于存储插入操作的数据。
 */
typedef struct InsertData{
    CString *tblname; ///< 要插入记录的表名称
    List  *vals;   ///< 要插入的值列表
    List *flds;    ///< 要插入的字段列表
}InsertData;

/**
 * @brief 初始化一个新的 InsertData 实例。
 *
 * @param tblname 要插入记录的表名称。
 * @param fld 要插入的字段列表。
 * @param vals 要插入的值列表。
 * @return 返回初始化后的 InsertData 指针。
 */
InsertData *InsertDataInit(CString *tblname,List *fld,List *vals);
#endif //DBMS_C_INSERTDATA_H
