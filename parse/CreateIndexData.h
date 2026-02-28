//
// Created by yaohuayuan on 2024/11/29.
//

#ifndef DBMS_C_CREATEINDEXDATA_H
#define DBMS_C_CREATEINDEXDATA_H
#include "CString.h"
/**
 * @brief CreateIndexData 结构体定义，用于存储创建索引的数据。
 */
typedef struct CreateIndexData{
    CString *idxname; ///< 索引名称
    CString *tblname; ///< 表名称
    CString *fldname; ///< 字段名称
}CreateIndexData;

/**
 * @brief 初始化一个新的 CreateIndexData 实例。
 *
 * @param idxname 索引名称。
 * @param tblname 表名称。
 * @param fldname 字段名称。
 * @return 返回初始化后的 CreateIndexData 指针。
 */
CreateIndexData *CreateIndexDataInit(CString *idxname,CString *tblname,CString *fldname);

#endif //DBMS_C_CREATEINDEXDATA_H
