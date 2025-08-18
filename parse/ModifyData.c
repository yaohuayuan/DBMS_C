//
// Created by yaohuayuan on 2024/11/30.
//

#include "ModifyData.h"
ModifyData *ModifyDataInit(char*tblname,char *fldname,Expression*newVal,Predicate*predicate){
    ModifyData *modifyData = malloc(sizeof(ModifyData));
    modifyData->predicate = predicate;
    modifyData->newVal =    newVal;
    modifyData->tblname = strdup(tblname);
    modifyData->fldname = strdup(fldname);
    return modifyData;
}