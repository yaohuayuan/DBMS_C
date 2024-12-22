//
// Created by yaohuayuan on 2024/11/30.
//

#ifndef DBMS_C_MODIFYDATA_H
#define DBMS_C_MODIFYDATA_H
#include "Expression.h"
#include "Predicate.h"
typedef struct ModifyData{
    char *tblname;
    char *fldname;
    Expression *newVal;
    Predicate *predicate;
}ModifyData;
ModifyData *ModifyDataInit(char*tblname,char *fldname,Expression*newVal,Predicate*predicate);
#endif //DBMS_C_MODIFYDATA_H
