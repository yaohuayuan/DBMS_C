//
// Created by yaohuayuan on 2024/11/30.
//

#include "DeleteData.h"
DeleteData *DeleteDataInit(char *tblname,Predicate*predicate){
    DeleteData * deleteData = malloc(sizeof (DeleteData));
    deleteData->predicate = predicate;
    deleteData->tblname = strdup(tblname);
    return deleteData;
}