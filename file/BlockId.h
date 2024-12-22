//
// Created by yaohuayuan on 2024/9/18.
//

#ifndef DBMS_C_BLOCKID_H
#define DBMS_C_BLOCKID_H

#include <stdbool.h>
#include "string.h"
#include <stdio.h>

// 定义 BlockID 结构体，用于唯一标识一个文件中的数据块。
typedef struct {
    char *fileName; // 数据块所在文件的名称。
    int blockId;    // 数据块在文件中的编号。
} BlockID;

/**
 * 初始化一个新的 BlockID 实例。
 *
 * @param b 指向要初始化的 BlockID 结构体。
 * @param name 文件名字符串。
 * @param id 数据块的编号。
 */
void BlockID_Init(BlockID *b, const char *name, int id);

/**
 * 获取 BlockID 对应的文件名。
 *
 * @param b BlockID 结构体。
 * @return 返回文件名字符串。
 */
char* getFileName(BlockID b);

/**
 * 获取 BlockID 对应的数据块编号。
 *
 * @param b BlockID 结构体。
 * @return 返回数据块编号。
 */
int getBlockId(BlockID b);

/**
 * 将 BlockID 转换为字符串表示形式。
 *
 * @param b BlockID 结构体。
 * @return 返回描述 BlockID 的字符串。
 */
char* BlockIDToString(BlockID b);

/**
 * 比较两个 BlockID 是否相等。
 *
 * @param b1 第一个 BlockID 结构体。
 * @param b2 第二个 BlockID 结构体。
 * @return 如果两个 BlockID 相等，则返回 true；否则返回 false。
 */
bool BlockIdEqual(BlockID b1, BlockID b2);

/**
 * 将字符串转换为 BlockID 结构体。
 *
 * @param str 描述 BlockID 的字符串。
 * @return 返回解析后的 BlockID 结构体。
 */
BlockID BloCKIDString2BlockID(char *str);

#endif //DBMS_C_BLOCKID_H