//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_TABLEMANAGER_H
#define DBMS_C_TABLEMANAGER_H
#include "Layout.h"
#include "Transaction.h"
#include "TableScan.h"
#define TABLE_MANAGER_MAX_NAME 16
typedef struct TableManager{
    Layout *tableCatalogLayout;
    Layout *fieldCatalogLayout;
}TableManager;
void TableManagerCreateTable(TableManager *tableManager,char*tblname,Schema*sch,Transaction*transaction);
TableManager *TableManagerInit(bool isNew,Transaction*transaction);
Layout *TableManagerGetLayout(TableManager *tableManager,char *tblname,Transaction *transaction);
#endif //DBMS_C_TABLEMANAGER_H
