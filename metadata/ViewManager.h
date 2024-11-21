//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_VIEWMANAGER_H
#define DBMS_C_VIEWMANAGER_H
#include "TableManager.h"
#define VIEW_MANAGER_MAX_VIEWDEF 100
typedef struct ViewManager{
    TableManager *tableManager;
}ViewManager;
ViewManager* ViewManagerInit(bool isNew, TableManager *tableManager, Transaction *transaction) ;

// 创建视图
void ViewManagerCreateView(ViewManager *viewManager, char *vname, char *vdef, Transaction *transaction);

char* ViewManagerGetViewDef(ViewManager *viewManager, char *vname, Transaction *transaction) ;
#endif //DBMS_C_VIEWMANAGER_H
