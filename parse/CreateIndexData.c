//
// Created by yaohuayuan on 2024/11/29.
//

#include "CreateIndexData.h"
#include "malloc.h"
#include "string.h"
CreateIndexData *CreateIndexDataInit(CString *idxname,CString *tblname,CString *fldname){
    CreateIndexData *createIndexData = malloc(sizeof(CreateIndexData));
    createIndexData->tblname = CStringCreateFromCString(tblname);
    createIndexData->fldname = CStringCreateFromCString(fldname);
    createIndexData->idxname = CStringCreateFromCString(idxname);
    return createIndexData;
}