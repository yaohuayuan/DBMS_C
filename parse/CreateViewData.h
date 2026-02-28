//
// Created by yaohuayuan on 2024/12/5.
//

#ifndef DBMS_C_CREATEVIEWDATA_H
#define DBMS_C_CREATEVIEWDATA_H
#include "QueryData.h"
/**
 * @brief CreateViewData 结构体定义，用于存储创建视图的数据。
 */
typedef struct CreateViewData{
    CString * viewName; ///< 视图名称
    QueryData *queryData; ///< 查询数据，包含视图的定义
}CreateViewData;

/**
 * @brief 初始化一个新的 CreateViewData 实例。
 *
 * @param viewname 视图名称。
 * @param queryData 查询数据，包含视图的定义。
 * @return 返回初始化后的 CreateViewData 指针。
 */
CreateViewData *CreateViewDataInit(CString *viewname,QueryData*queryData);
#endif //DBMS_C_CREATEVIEWDATA_H
