//
// Created by yaohuayuan on 2024/11/28.
//

#include "InsertData.h"
#include "List.h"
InsertData *InsertDataInit(char *tblname,List *fld,List *vals){
    InsertData *insertData = malloc(sizeof(InsertData));
    insertData->tblname = strdup(tblname);
    insertData->flds = fld;
    insertData->vals = vals;
    return insertData;
}