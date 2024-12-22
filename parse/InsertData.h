//
// Created by yaohuayuan on 2024/11/28.
//

#ifndef DBMS_C_INSERTDATA_H
#define DBMS_C_INSERTDATA_H
#include "List.h"
typedef struct InsertData{
    char *tblname;
    List  *vals;
    List *flds;
}InsertData;
InsertData *InsertDataInit(char *tblname,List *fld,List *vals);
#endif //DBMS_C_INSERTDATA_H
