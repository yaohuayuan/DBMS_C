//
// Created by yaohuayuan on 2024/9/18.
//
#include <stdlib.h>
#include "BlockId.h"

void BlockID_Init(BlockID *b,char *name,int id){
    b->fileName = name;
    b->blockId = id;
}
char * getFileName(BlockID b){
    return b.fileName;
}
int getBlockId(BlockID b){
    return b.blockId;
}
char* display(BlockID b) {
    char *str = malloc(256); // 分配足够的内存
    if (str == NULL)
        return NULL; // 检查分配是否成功
    snprintf(str, 256, "This BlockID is file: %s, ID: %d", getFileName(b), b.blockId);
    return str;
}

bool BlockIdEqual(BlockID b1,BlockID b2){
    return (b1.fileName == b2.fileName) && (b1.blockId == b2.blockId);
}