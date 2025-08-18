//
// Created by yaohuayuan on 2024/11/29.
//

#ifndef DBMS_C_CREATETABLEDATA_H
#define DBMS_C_CREATETABLEDATA_H
#include "Schema.h"
typedef struct CreateTableData{
    char *tblname;
    Schema *schema;
}CreateTableData;
CreateTableData* CreateTableDataInit(char *tblname,Schema*schema);
#endif //DBMS_C_CREATETABLEDATA_H
