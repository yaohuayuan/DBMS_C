//
// Created by yaohuayuan on 2024/10/31.
//

#ifndef DBMS_C_LAYOUT_H
#define DBMS_C_LAYOUT_H
#include "Schema.h"
#include"Page.h"
#include "CString.h"
/**
 * @brief Layout 结构体，用于表示表的布局信息，包括模式、字段偏移量和记录槽大小。
 */
typedef struct Layout{
    Schema *schema;    ///< 表的模式信息
    map_int_t *offsets; ///< 字段名到偏移量的映射
    int SlotSize;      ///< 每个记录槽的大小（字节）
}Layout;

/**
 * @brief 初始化一个新的 Layout 实例。
 *
 * @param schema 表的模式信息。
 * @param mapInt 字段名到偏移量的映射。
 * @param SloSize 每个记录槽的大小（字节）。
 * @return 返回初始化后的 Layout 指针。
 */
Layout * LayoutInit(Schema*schema,map_int_t* mapInt,int SloSize );

/**
 * @brief 获取指定字段名的偏移量。
 *
 * @param layout 指向 Layout 实例的指针。
 * @param FldName 字段名。
 * @return 返回字段的偏移量。
 */
int LayoutOffset(Layout* layout, CString *FldName);
#endif //DBMS_C_LAYOUT_H
