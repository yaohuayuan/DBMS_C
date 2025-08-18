//
// Created by yaohuayuan on 2024/10/31.
//

#ifndef DBMS_C_LAYOUT_H
#define DBMS_C_LAYOUT_H
#include "Schema.h"
#include"Page.h"
typedef struct Layout{
    Schema *schema;
    map_int_t *offsets;
    int SlotSize;
}Layout;
Layout * LayoutInit(Schema*schema,map_int_t* mapInt,int SloSize );
int LayoutOffset(Layout*layout ,char*FldName);
#endif //DBMS_C_LAYOUT_H
