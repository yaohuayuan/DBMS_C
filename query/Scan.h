//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_SCAN_H
#define DBMS_C_SCAN_H

#include "Constant.h"
#include "Schema.h"
#include "TableScan.h"
#include "ProductScan.h"
#include <stdbool.h>
#include "SelectScan.h"
#include "ProjectScan.h"
#include "CString.h"
typedef struct ProjectScan ProjectScan;
typedef struct TableScan TableScan;
typedef struct ProductScan ProductScan;
typedef struct SelectScan SelectScan;
typedef struct Scan Scan;
typedef struct RID RID;
/**
 * @brief ScanOps 结构体，包含扫描操作的函数指针集合。
 */
typedef struct ScanOps {
    void (*beforeFirst)(void *scan);   ///< 将扫描定位到第一个记录之前
    bool (*next)(void *scan);          ///< 移动到下一个记录，返回是否成功
    int (*getInt)(void *scan,  CString *fldname); ///< 获取指定字段的整数值
    char* (*getString)(void *scan,  CString *fldname); ///< 获取指定字段的字符串值
    Constant* (*getVal)(void *scan,  CString *fldname); ///< 获取指定字段的常量值
    bool (*hasField)(void *scan,  CString *fldname); ///< 检查是否存在指定字段
    void (*close)(void *scan);         ///< 关闭扫描，释放资源
} ScanOps;

/**
 * @brief ScanCode 枚举，定义了不同类型的扫描。
 */
typedef enum ScanCode {
    SCAN_TABLE_CODE,      ///< 表扫描类型
    SCAN_PRODUCT_CODE,    ///< 乘积扫描类型
    SCAN_SELECT_CODE,     ///< 选择扫描类型
    SCAN_PROJECT_CODE,    ///< 投影扫描类型
} ScanCode;

/**
 * @brief ScanUnion 联合类型，用于存储不同类型的扫描实例。
 */
typedef union ScanUnion {
    TableScan *tableScan;      ///< 表扫描实例
    ProductScan*productScan;   ///< 乘积扫描实例
    SelectScan *selectScan;    ///< 选择扫描实例
    ProjectScan *projectScan;  ///< 投影扫描实例
} ScanUnion;

/**
 * @brief Scan 结构体，用于表示数据库中的扫描操作。
 */
typedef struct Scan {
    void (*beforeFirst)(void *scan);   ///< 将扫描定位到第一个记录之前
    bool (*next)(void *scan);          ///< 移动到下一个记录，返回是否成功
    int (*getInt)(void *scan,  CString *fldname); ///< 获取指定字段的整数值
    char* (*getString)(void *scan,  CString *fldname); ///< 获取指定字段的字符串值
    Constant* (*getVal)(void *scan,  CString *fldname); ///< 获取指定字段的常量值
    bool (*hasField)(void *scan,  CString *fldname); ///< 检查是否存在指定字段
    void (*close)(void *scan);         ///< 关闭扫描，释放资源
    void (*setVal)(void *data,CString *fldname,Constant*constant); ///< 设置指定字段的常量值
    void (*setInt)(void *data,CString *fldname,int val); ///< 设置指定字段的整数值
    void (*setString)(void *data,CString *fldname,CString *val); ///< 设置指定字段的字符串值
    void (*insert)(void*data);         ///< 插入新记录
    void (*delete)(void *data);        ///< 删除当前记录
    RID *(*getRid)(void *data);        ///< 获取当前记录的RID
    void (*moveToRID)(void *data,RID *rid); ///< 移动到指定RID的记录
    ScanUnion scanUnion;              ///< 扫描实例联合
    ScanCode code;                    ///< 扫描类型
} Scan;

/**
 * @brief 初始化一个新的 Scan 实例。
 *
 * @param data 扫描数据指针。
 * @param code 扫描类型。
 * @return 返回初始化后的 Scan 指针。
 */
Scan *ScanInit(void *data,ScanCode code);
#endif //DBMS_C_SCAN_H
