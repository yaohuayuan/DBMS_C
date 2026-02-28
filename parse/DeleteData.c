//
// Created by yaohuayuan on 2024/11/30.
//

#include "DeleteData.h"
#include "CString.h"
DeleteData *DeleteDataInit(CString *tblname,Predicate*predicate){
    DeleteData * deleteData = malloc(sizeof (DeleteData));
    deleteData->predicate = predicate;
    deleteData->tblname = CStringCreateFromCString(tblname);
    return deleteData;
}