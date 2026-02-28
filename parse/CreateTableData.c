//
// Created by yaohuayuan on 2024/11/29.
//

#include "CreateTableData.h"
#include "CString.h"
CreateTableData* CreateTableDataInit(CString *tblname,Schema*schema){
    CreateTableData *createTableData = malloc(sizeof (CreateTableData));
    createTableData->schema = schema;
    createTableData->tblname = CStringCreateFromCString(tblname);
    return createTableData;
}