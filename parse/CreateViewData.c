//
// Created by yaohuayuan on 2024/12/5.
//

#include "CreateViewData.h"
#include "CString.h"
CreateViewData *CreateViewDataInit(CString *viewname,QueryData*queryData){
    CreateViewData *createViewData = malloc(sizeof (CreateViewData));
    createViewData->viewName = CStringCreateFromCString(viewname);
    createViewData->queryData = queryData;
    return createViewData;
}