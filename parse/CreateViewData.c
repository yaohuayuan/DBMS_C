//
// Created by yaohuayuan on 2024/12/5.
//

#include "CreateViewData.h"
CreateViewData *CreateViewDataInit(char *string,QueryData*queryData){
    CreateViewData *createViewData = malloc(sizeof (CreateViewData));
    createViewData->viewName = strdup(string);
    createViewData->queryData = queryData;
    return createViewData;
}