//
// Created by yaohuayuan on 2025/5/30.
//

#ifndef NEWDBMS_BLOCKID_H
#define NEWDBMS_BLOCKID_H
#include "CString.h"
#include "stdbool.h"
typedef struct BlockID {
    CString *fileName;
    int BlockID;
}BlockID;

/**
 * 初始化一个新的 BlockID 实例。
 *
 * @param b 指向要初始化的 BlockID 结构体。
 * @param name 文件名字符串。
 * @param id 数据块的编号。
 */
BlockID *block_id_init(CString *name, int id);

/**
 * 获取 BlockID 对应的文件名。
 *
 * @param b BlockID 结构体。
 * @return 返回文件名字符串。
 */
CString* blockId_getFileName(BlockID *b);

/**
 * 获取 BlockID 对应的数据块编号。
 *
 * @param b BlockID 结构体。
 * @return 返回数据块编号。
 */
int blockId_getBlockId(BlockID *b);

/**
 * 将 BlockID 转换为字符串表示形式。
 *
 * @param b BlockID 结构体。
 * @return 返回描述 BlockID 的字符串。
 */
CString* blockId_to_CString(BlockID *b);

/**
 * 比较两个 BlockID 是否相等。
 *
 * @param b1 第一个 BlockID 结构体。
 * @param b2 第二个 BlockID 结构体。
 * @return 如果两个 BlockID 相等，则返回 true；否则返回 false。
 */
bool blockId_equal(BlockID *b1, BlockID *b2);

/**
 * 将字符串转换为 BlockID 结构体。
 *
 * @param str 描述 BlockID 的字符串。
 * @return 返回解析后的 BlockID 结构体。
 */
BlockID* blockKIdCString2BlockID(CString *str);
#endif //NEWDBMS_BLOCKID_H
