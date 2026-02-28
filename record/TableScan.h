//
// Created by yaohuayuan on 2024/11/15.
//

#ifndef DBMS_C_TABLESCAN_H
#define DBMS_C_TABLESCAN_H
#include "Transaction.h"
#include "Layout.h"
#include "RecordPage.h"
#include "RID.h"
#include "Constant.h"
#include "UpdateScan.h"
#include "CString.h"
/**
 * @brief TableScan 结构体，用于表示表扫描操作，提供对表记录的遍历、插入、删除和更新功能。
 */
typedef struct TableScan{
    Transaction *transaction;  ///< 事务指针
    Layout *layout;           ///< 表布局
    RecordPage *recordPage;    ///< 当前记录页
    CString *fileName;           ///< 文件名
    int currentSlot;          ///< 当前槽号
//    UpdateScan *updateScan;   ///< 更新扫描器（已注释）
}TableScan;

/**
 * @brief 关闭表扫描，释放资源。
 *
 * @param data 指向 TableScan 实例的指针。
 */
void TableScanClose(void *data);

/**
 * @brief 移动到新的块。
 *
 * @param tableScan 指向 TableScan 实例的指针。
 */
void TableScanMoveToNewBlock(TableScan*tableScan);

/**
 * @brief 移动到指定块。
 *
 * @param tableScan 指向 TableScan 实例的指针。
 * @param blkNum 块号。
 */
void TableScanMoveToBlock(TableScan*tableScan,int blkNum);

/**
 * @brief 初始化一个新的 TableScan 实例。
 *
 * @param transaction 事务指针。
 * @param TBName 表名。
 * @param layout 表布局。
 * @return 返回初始化后的 TableScan 指针。
 */
TableScan *TableScanInit(Transaction*transaction,CString*TBName,Layout*layout);

/**
 * @brief 将扫描定位到第一个记录之前。
 *
 * @param data 指向 TableScan 实例的指针。
 */
void TableScanBeforeFirst(void *data);

/**
 * @brief 检查是否在最后一个块。
 *
 * @param tableScan 指向 TableScan 实例的指针。
 * @return 如果在最后一个块，返回 true；否则返回 false。
 */
bool TableScanAtLastBlock(TableScan *tableScan);

/**
 * @brief 移动到下一个记录。
 *
 * @param data 指向 TableScan 实例的指针。
 * @return 如果成功移动到下一个记录，返回 true；否则返回 false。
 */
bool TableScanNext(void *data);

/**
 * @brief 插入新记录。
 *
 * @param data 指向 TableScan 实例的指针。
 */
void TableScanInsert(void *data);

/**
 * @brief 删除当前记录。
 *
 * @param data 指向 TableScan 实例的指针。
 */
void TableScanDelete(void *data);

/**
 * @brief 移动到指定 RID 的记录。
 *
 * @param data 指向 TableScan 实例的指针。
 * @param rid 记录标识符。
 */
void TableScanMoveToRid(void *data,RID *rid);

/**
 * @brief 检查是否存在指定字段。
 *
 * @param data 指向 TableScan 实例的指针。
 * @param fileNam 字段名。
 * @return 如果存在该字段，返回 true；否则返回 false。
 */
bool TableScanHasField(void *data,CString*fileNam);

/**
 * @brief 获取指定字段的整数值。
 *
 * @param data 指向 TableScan 实例的指针。
 * @param fileName 字段名。
 * @return 返回字段的整数值。
 */
int TableScanGetInt(void *data,CString *fileName);

/**
 * @brief 获取指定字段的字符串值。
 *
 * @param data 指向 TableScan 实例的指针。
 * @param fileName 字段名。
 * @return 返回字段的字符串值。
 */
char* TableScanGetString(void *data,CString *fileName);

/**
 * @brief 设置指定字段的整数值。
 *
 * @param data 指向 TableScan 实例的指针。
 * @param fileName 字段名。
 * @param idata 要设置的整数值。
 */
void TableScanSetInt(void *data,CString*fileName,int idata);

/**
 * @brief 设置指定字段的字符串值。
 *
 * @param data 指向 TableScan 实例的指针。
 * @param fileName 字段名。
 * @param sdata 要设置的字符串值。
 */
void TableScanSetString(void *data,CString*fileName,CString* sdata);

/**
 * @brief 获取当前记录的 RID。
 *
 * @param data 指向 TableScan 实例的指针。
 * @return 返回当前记录的 RID 指针。
 */
RID* TableScanGetRID(void *data);

/**
 * @brief 获取指定字段的常量值。
 *
 * @param data 指向 TableScan 实例的指针。
 * @param fldname 字段名。
 * @return 返回字段的常量值指针。
 */
Constant* TableScanGetVal(void *data,CString *fldname);

/**
 * @brief 设置指定字段的常量值。
 *
 * @param data 指向 TableScan 实例的指针。
 * @param fldname 字段名。
 * @param val 要设置的常量值指针。
 */
void TableScanSetVal(void *data,CString *fldname,Constant*val);
#endif //DBMS_C_TABLESCAN_H
