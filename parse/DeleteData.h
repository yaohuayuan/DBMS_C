//
// Created by yaohuayuan on 2024/11/30.
//

#ifndef DBMS_C_DELETEDATA_H
#define DBMS_C_DELETEDATA_H
#include "Predicate.h"
typedef struct DeleteData{
    char *tblname;
    Predicate *predicate;
}DeleteData;
DeleteData *DeleteDataInit(char *tblname,Predicate*predicate);
#endif //DBMS_C_DELETEDATA_H
