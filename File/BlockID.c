//
// Created by yaohuayuan on 2025/5/30.
//

#include "BlockID.h"
BlockID *block_id_init(CString *name, int id){
    BlockID *b = (BlockID *)malloc(sizeof(BlockID));
    b->fileName = c_string_copy(name);
    b->BlockID = id;
    return b;
}
CString* blockId_getFileName(BlockID *b){
    return c_string_copy(b->fileName);
}
int blockId_getBlockId(BlockID *b){
    return b->BlockID;
}

CString* blockId_to_CString(BlockID *b){
    CString *c_string = c_string_copy(b->fileName);
    c_string_append(c_string,c_string_init(":"));
    int counter = b->BlockID;
    while (counter){
        char *num = (char *)malloc(2*sizeof(char));
        num[0] = counter%10+'0';
        num[1] = '\0';
        CString *c_string_num = c_string_init(num);
        c_string_append(c_string,c_string_num);
        counter /= 10;
    }
    return c_string;
}

bool blockId_equal(BlockID *b1, BlockID *b2){
    return blockId_getBlockId(b1) == blockId_getBlockId(b2) && c_string_compare(blockId_getFileName(b1),blockId_getFileName(b2)) == 0;
}


BlockID* blockKIdCString2BlockID(CString *str){
    int i;
    for (i = 0; i < str->length; i++) {
        if (str->data[i] == ':') {
            break;
        }
    }

    // 如果没找到 ':'，返回 NULL
    if (i == str->length) {
        return NULL;
    }

    // 拷贝文件名部分
    char *file_part = (char *)malloc(i + 1);
    strncpy(file_part, str->data, i);
    file_part[i] = '\0';  // 末尾加 \0
    CString *file_name = c_string_init(file_part);
    free(file_part);  // c_string_init 里会复制值

    // 拷贝 block ID 数字部分
    int block_id = 0;
    for (int j = i + 1; j < str->length; j++) {
        if (str->data[j] >= '0' && str->data[j] <= '9') {
            block_id = block_id * 10 + (str->data[j] - '0');
        } else {
            // 非法字符
            c_string_free(file_name);
            return NULL;
        }
    }

    return block_id_init(file_name, block_id);

}