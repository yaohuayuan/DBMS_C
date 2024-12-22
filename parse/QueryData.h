//
// Created by yaohuayuan on 2024/11/30.
//

#ifndef DBMS_C_QUERYDATA_H
#define DBMS_C_QUERYDATA_H
#include "List.h"
#include "Predicate.h"
typedef struct QueryData{
    List *fields;
    List* tables;
    Predicate *predicate;
}QueryData;
QueryData *QueryDataInit(List*fields,List*tables,Predicate *predicate);
char *QueryDataToString(QueryData*queryData);
#endif //DBMS_C_QUERYDATA_H
