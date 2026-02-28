//
// Created by yaohuayuan on 2024/11/14.
//

#ifndef DBMS_C_RECORDPAGE_H
#define DBMS_C_RECORDPAGE_H
#include "Transaction.h"
#include "Layout.h"
#include "CString.h"
/**
 * @brief RecordCode 枚举，定义了记录页的状态。
 */
typedef enum RecordPageCode{
    RECORD_PAGE_EMPTY,  ///< 记录页为空
    RECORD_PAGE_USED    ///< 记录页已使用
}RecordCode;

/**
 * @brief RecordPage 结构体，用于表示数据库中的记录页。
 */
typedef struct RecordPage{
    Transaction *transaction;  ///< 事务指针
    BlockID *blockId;         ///< 块ID
    Layout *layout;           ///< 记录布局
}RecordPage;

/**
 * @brief 初始化一个新的 RecordPage 实例。
 *
 * @param transaction 事务指针。
 * @param blockId 块ID。
 * @param layout 记录布局。
 * @return 返回初始化后的 RecordPage 指针。
 */
RecordPage * RecordPageInit(Transaction*transaction,BlockID *blockId,Layout*layout);

/**
 * @brief 获取指定槽的偏移量。
 *
 * @param recordPage 指向 RecordPage 实例的指针。
 * @param slot 槽号。
 * @return 返回槽的偏移量。
 */
int RecordPageOffset(RecordPage *recordPage,int slot);

/**
 * @brief 获取指定槽和字段的整数值。
 *
 * @param recordPage 指向 RecordPage 实例的指针。
 * @param slot 槽号。
 * @param fldName 字段名。
 * @return 返回整数值。
 */
int RecordPageGetInt(RecordPage *recordPage,int slot,CString *fldName);

/**
 * @brief 获取指定槽和字段的字符串值。
 *
 * @param recordPage 指向 RecordPage 实例的指针。
 * @param slot 槽号。
 * @param fldName 字段名。
 * @return 返回字符串值。
 */
const char *RecordPageGetString(RecordPage *recordPage,int slot,CString*fldName);

/**
 * @brief 设置指定槽和字段的整数值。
 *
 * @param recordPage 指向 RecordPage 实例的指针。
 * @param slot 槽号。
 * @param fldName 字段名。
 * @param val 要设置的整数值。
 */
void RecordSetInt(RecordPage*recordPage,int slot,CString *fldName,int val);

/**
 * @brief 设置指定槽和字段的字符串值。
 *
 * @param recordPage 指向 RecordPage 实例的指针。
 * @param slot 槽号。
 * @param fldName 字段名。
 * @param val 要设置的字符串值。
 */
void RecordSetString(RecordPage*recordPage,int slot,CString *fldName,CString* val);

/**
 * @brief 设置指定槽的标志。
 *
 * @param recordPage 指向 RecordPage 实例的指针。
 * @param slot 槽号。
 * @param flag 要设置的标志。
 */
void RecordPageSetFlag(RecordPage*recordPage,int slot,int flag);

/**
 * @brief 删除指定槽的记录。
 *
 * @param recordPage 指向 RecordPage 实例的指针。
 * @param slot 槽号。
 */
void RecordPageDelete(RecordPage*recordPage,int slot);

/**
 * @brief 检查指定槽是否有效。
 *
 * @param recordPage 指向 RecordPage 实例的指针。
 * @param slot 槽号。
 * @return 如果槽有效，返回 true；否则返回 false。
 */
bool RecordPageIsValidSlot(RecordPage*recordPage,int slot);

/**
 * @brief 在指定槽之后搜索具有特定标志的记录。
 *
 * @param recordPage 指向 RecordPage 实例的指针。
 * @param slot 起始槽号。
 * @param flag 要搜索的标志。
 * @return 返回找到的槽号，未找到则返回-1。
 */
int RecordPageSearchAfter(RecordPage*recordPage,int slot,int flag);

/**
 * @brief 格式化记录页。
 *
 * @param recordPage 指向 RecordPage 实例的指针。
 */
void RecordPageFormat(RecordPage *recordPage);

/**
 * @brief 在指定槽之后插入新记录。
 *
 * @param recordPage 指向 RecordPage 实例的指针。
 * @param slot 起始槽号。
 * @return 返回插入的槽号，插入失败则返回-1。
 */
int RecordPageInsertAfter(RecordPage*recordPage,int slot);

/**
 * @brief 获取指定槽之后的下一个槽。
 *
 * @param recordPage 指向 RecordPage 实例的指针。
 * @param slot 起始槽号。
 * @return 返回下一个槽号。
 */
int RecordPageNextAfter(RecordPage*recordPage,int slot);
#endif //DBMS_C_RECORDPAGE_H
