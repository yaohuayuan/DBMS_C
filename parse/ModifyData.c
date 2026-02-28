//
// Created by yaohuayuan on 2024/11/30.
//

#include "ModifyData.h"
#include "CString.h"
ModifyData *ModifyDataInit(CString*tblname,CString *fldname,Expression*newVal,Predicate*predicate){
    ModifyData *modifyData = malloc(sizeof(ModifyData));
    modifyData->predicate = predicate;
    modifyData->newVal =    newVal;
    modifyData->tblname = CStringCreateFromCString(tblname);
    modifyData->fldname = CStringCreateFromCString(fldname);
    return modifyData;
}