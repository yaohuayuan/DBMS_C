//
// Created by yaohuayuan on 2024/11/29.
//

#include "CreateIndexData.h"
#include "malloc.h"
#include "string.h"
CreateIndexData *CreateIndexDataInit(char *idxname,char *tblname,char *fldname){
    CreateIndexData *createIndexData = malloc(sizeof(CreateIndexData));
    createIndexData->tblname = strdup(tblname);
    createIndexData->fldname = strdup(fldname);
    createIndexData->idxname = strdup(idxname);
    return createIndexData;
}