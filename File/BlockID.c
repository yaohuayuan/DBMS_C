//
// Created by yaohuayuan on 2025/5/30.
//

#include "BlockID.h"
#include "stdio.h"
BlockID *BlockIDInit(CString *name, int id){
    if (!name)
        return NULL;
    BlockID *b = (BlockID *)malloc(sizeof(BlockID));
    b->fileName = CStringCreateFromCString(name);
    b->BlockID = id;
    return b;
}
CString* BlockIDGetFileName(const BlockID *b){
    return b->fileName;
}


CString* BlockID2CString(BlockID *b) {
    CString *CStr = CStringCreateFromCString(b->fileName);
    CStringAppendCStr(CStr, ": ");
    char numStr[20];
    snprintf(numStr, sizeof(numStr), "%d", b->BlockID);  // 格式化 BlockID 为字符串
    CString *blockIDStr = CStringCreateFromCStr(numStr);
    CStringAppendCString(CStr, blockIDStr);
    CStringDestroy(blockIDStr);
    return CStr;
}


bool BlockIDEqual(BlockID *b1, BlockID *b2){
    return BlockIDGetBlockID(b1) == BlockIDGetBlockID(b2)
    && CStringEqual(BlockIDGetFileName(b1),BlockIDGetFileName(b2));
}


BlockID* BlockIDCString2BlockID(CString *str) {
    int pos = CStringFind(str, ":");
    if (pos < 0 || pos + 1 >= (int)str->length) {
        return NULL;
    }
    CString *file_name = CStringSubstring(str, 0, pos);
    int start = pos + 1;
    while (start < (int)str->length && str->data[start] == ' ') {
        start++;
    }
    char numStr[32] = {0};
    int len = 0;
    for (int i = start; i < (int)str->length && len < 31; i++) {
        if ((str->data[i] >= '0' && str->data[i] <= '9') || str->data[i] == '-') {
            numStr[len++] = str->data[i];
        } else {
            break;
        }
    }

    int block_id = atoi(numStr);
    BlockID *blk = BlockIDInit(file_name, block_id);
    CStringDestroy(file_name);
    return blk;
}
void BlockIDDestroy(BlockID *blockId){
    CStringDestroy(blockId->fileName);
    free(blockId);
}
int BlockIDGetBlockID(const BlockID *b){
    return b->BlockID;
}