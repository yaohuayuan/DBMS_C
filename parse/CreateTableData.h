//
// Created by yaohuayuan on 2024/11/29.
//

#ifndef DBMS_C_CREATETABLEDATA_H
#define DBMS_C_CREATETABLEDATA_H
#include "Schema.h"
/**
 * @brief CreateTableData 结构体定义，用于存储创建表的数据。
 */
typedef struct CreateTableData{
    CString *tblname; ///< 表名称
    Schema *schema; ///< 表的模式信息
}CreateTableData;

/**
 * @brief 初始化一个新的 CreateTableData 实例。
 *
 * @param tblname 表名称。
 * @param schema 表的模式信息。
 * @return 返回初始化后的 CreateTableData 指针。
 */
CreateTableData* CreateTableDataInit(CString *tblname,Schema*schema);
#endif //DBMS_C_CREATETABLEDATA_H
