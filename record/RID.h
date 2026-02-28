//
// Created by yaohuayuan on 2024/10/31.
//

#ifndef DBMS_C_RID_H
#define DBMS_C_RID_H
#include "malloc.h"
#include "stdbool.h"
#include "string.h"
#include <stdio.h>
/**
 * @brief RID 结构体，用于表示记录标识符，包含块号和槽号。
 */
typedef struct RID{
    int BlockNum; ///< 记录所在的块号
    int Slot;     ///< 记录在块内的槽号
}RID;

/**
 * @brief 初始化一个新的 RID 实例。
 *
 * @param BlockNum 记录所在的块号。
 * @param Slot 记录在块内的槽号。
 * @return 返回初始化后的 RID 指针。
 */
RID* RIDInit(int BlockNum,int Slot);

/**
 * @brief 检查两个 RID 是否相等。
 *
 * @param rid1 指向第一个 RID 实例的指针。
 * @param rid2 指向第二个 RID 实例的指针。
 * @return 如果两个 RID 相等，返回 true；否则返回 false。
 */
bool RIDEqual(RID *rid1,RID *rid2);

/**
 * @brief 将 RID 转换为字符串表示。
 *
 * @param rid 指向 RID 实例的指针。
 * @return 返回 RID 的字符串表示。
 */
char *RIDToString(RID*rid);

#endif //DBMS_C_RID_H
