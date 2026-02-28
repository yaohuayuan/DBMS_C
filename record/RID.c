//
// Created by yaohuayuan on 2024/10/31.
//

#include "RID.h"
RID* RIDInit(int BlockNum,int Slot){
    RID  *rid = malloc(sizeof(RID));
    rid->BlockNum = BlockNum;
    rid->Slot = Slot;
    return rid;
}
bool RIDEqual(RID *rid1,RID *rid2){
    return rid1->Slot==rid2->Slot&&rid1->BlockNum==rid2->BlockNum;
}
char *RIDToString(RID*rid){
    char *str = malloc(20); // 足够大的缓冲区
    if (str == NULL)
        return NULL; // 检查分配是否成功
    snprintf(str, 20, "(%d,%d)", rid->BlockNum, rid->Slot);
    return str;
}