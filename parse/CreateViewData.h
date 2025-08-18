//
// Created by yaohuayuan on 2024/12/5.
//

#ifndef DBMS_C_CREATEVIEWDATA_H
#define DBMS_C_CREATEVIEWDATA_H
#include "QueryData.h"
typedef struct CreateViewData{
    char * viewName;
    QueryData *queryData;
}CreateViewData;
CreateViewData *CreateViewDataInit(char *string,QueryData*queryData);
#endif //DBMS_C_CREATEVIEWDATA_H
