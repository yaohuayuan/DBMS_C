//
// Created by yaohuayuan on 2024/9/18.
//

#ifndef DBMS_C_BLOCKID_H
#define DBMS_C_BLOCKID_H

#include <stdbool.h>
#include "string.h"
#include<stdio.h>
typedef struct {
    char *fileName;
    int blockId;
}BlockID;
// 初始化BlockID
void BlockID_Init(BlockID *b,char * name,int id);
char * getFileName(BlockID b);
int getBlockId(BlockID);
char * BlockIDToString(BlockID);
bool BlockIdEqual(BlockID b1,BlockID b2);
BlockID BloCKIDString2BlockID(char * str);
#endif //DBMS_C_BLOCKID_H
