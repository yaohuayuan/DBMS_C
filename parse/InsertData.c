//
// Created by yaohuayuan on 2024/11/28.
//

#include "InsertData.h"
#include "List.h"
#include "CString.h"
InsertData *InsertDataInit(CString *tblname,List *fld,List *vals){
    InsertData *insertData = malloc(sizeof(InsertData));
    insertData->tblname = CStringCreateFromCString(tblname);
    insertData->flds = fld;
    insertData->vals = vals;
    return insertData;
}