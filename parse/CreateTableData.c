//
// Created by yaohuayuan on 2024/11/29.
//

#include "CreateTableData.h"
CreateTableData* CreateTableDataInit(char *tblname,Schema*schema){
    CreateTableData *createTableData = malloc(sizeof (CreateTableData));
    createTableData->schema = schema;
    createTableData->tblname = strdup(tblname);
    return createTableData;
}