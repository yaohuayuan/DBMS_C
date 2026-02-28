//
// Created by yaohuayuan on 2024/11/30.
//

#ifndef DBMS_C_DELETEDATA_H
#define DBMS_C_DELETEDATA_H
#include "Predicate.h"
/**
 * @brief DeleteData 结构体定义，用于存储删除操作的数据。
 */
typedef struct DeleteData{
    CString *tblname; ///< 要删除记录的表名称
    Predicate *predicate; ///< 删除条件，只有满足条件的记录才会被删除
}DeleteData;

/**
 * @brief 初始化一个新的 DeleteData 实例。
 *
 * @param tblname 要删除记录的表名称。
 * @param predicate 删除条件，只有满足条件的记录才会被删除。
 * @return 返回初始化后的 DeleteData 指针。
 */
DeleteData *DeleteDataInit(CString *tblname,Predicate*predicate);
#endif //DBMS_C_DELETEDATA_H
