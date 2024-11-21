//
// Created by yaohuayuan on 2024/10/31.
//

#ifndef DBMS_C_RID_H
#define DBMS_C_RID_H
#include "malloc.h"
#include "stdbool.h"
#include "string.h"
#include <stdio.h>
typedef struct RID{
    int BlockNum;
    int Slot;
}RID;
RID* RIDInit(int BlockNum,int Slot);
bool RIDEqual(RID *rid1,RID *rid2);
char *RIDToString(RID*rid);
#endif //DBMS_C_RID_H
