// Created by yaohuayuan on 2024/11/19.

#ifndef DBMS_C_VIEWMANAGER_H
#define DBMS_C_VIEWMANAGER_H

#include "TableManager.h"
#include "CString.h"

#define VIEW_MANAGER_MAX_VIEWDEF 100 ///< 定义视图定义最大长度

/**
 * @brief ViewManager 结构体，用于管理数据库中的视图。
 */
typedef struct ViewManager {
    TableManager *tableManager; ///< 指向表管理器的指针
} ViewManager;

/**
 * @brief 初始化一个新的 ViewManager 实例。
 *
 * @param isNew 是否是新创建的实例，如果是新创建的则为 true。
 * @param tableManager 指向 TableManager 的指针。
 * @param transaction 当前事务的指针。
 * @return 返回初始化后的 ViewManager 指针。
 */
ViewManager* ViewManagerInit(bool isNew, TableManager *tableManager, Transaction *transaction);

/**
 * @brief 创建一个新的视图。
 *
 * @param viewManager 指向 ViewManager 的指针。
 * @param vname 视图名称。
 * @param vdef 视图定义（SQL语句）。
 * @param transaction 当前事务的指针。
 */
void ViewManagerCreateView(ViewManager *viewManager, CString *vname, CString *vdef, Transaction *transaction);

/**
 * @brief 获取指定视图的定义。
 *
 * @param viewManager 指向 ViewManager 的指针。
 * @param vname 视图名称。
 * @param transaction 当前事务的指针。
 * @return 返回视图的定义字符串。
 */
CString* ViewManagerGetViewDef(ViewManager *viewManager, CString *vname, Transaction *transaction);

#endif //DBMS_C_VIEWMANAGER_H
