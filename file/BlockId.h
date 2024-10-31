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
char * display(BlockID);
bool BlockIdEqual(BlockID b1,BlockID b2);
#endif //DBMS_C_BLOCKID_H
