
#ifndef DBMS_C_SCHEMA_H
#define DBMS_C_SCHEMA_H

#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "CString.h"
#include "map.h"
#include <stdbool.h>
/**
 * @brief FileInfoCode 枚举，定义了各种文件信息类型。
 */
typedef enum {
    FILE_INFO_CODE_BIT,              ///< 位类型
    FILE_INFO_CODE_TINYINT,          ///< 微型整数类型
    FILE_INFO_CODE_SMALLINT,         ///< 小型整数类型
    FILE_INFO_CODE_INTEGER,          ///< 整数类型
    FILE_INFO_CODE_BIGINT,           ///< 大型整数类型
    FILE_INFO_CODE_FLOAT,            ///< 单精度浮点数类型
    FILE_INFO_CODE_REAL,             ///< 实数类型
    FILE_INFO_CODE_DOUBLE,           ///< 双精度浮点数类型
    FILE_INFO_CODE_NUMERIC,          ///< 数值类型
    FILE_INFO_CODE_DECIMAL,          ///< 十进制类型
    FILE_INFO_CODE_CHAR,             ///< 定长字符类型
    FILE_INFO_CODE_VARCHAR,          ///< 变长字符类型
    FILE_INFO_CODE_LONG_VARCHAR,     ///< 长变长字符类型
    FILE_INFO_CODE_DATE,             ///< 日期类型
    FILE_INFO_CODE_TIME,             ///< 时间类型
    FILE_INFO_CODE_TIMESTAMP,        ///< 时间戳类型
    FILE_INFO_CODE_BINARY,           ///< 二进制类型
    FILE_INFO_CODE_OTHER,            ///< 其他类型
    FILE_INFO_CODE_NULL,             ///< 空类型
} FileInfoCode;

/**
 * @brief FileInfo 结构体，用于表示文件信息，包含类型和长度。
 */
typedef struct FileInfo {
    int type;   ///< 文件类型，对应 FileInfoCode 枚举
    int length; ///< 文件长度
} FileInfo;

/**
 * @brief 定义文件信息映射类型。
 */
typedef map_t(FileInfo) map_FileInfo_t;

/**
 * @brief FieldNode 结构体，用于表示字段节点，包含字段名、类型、长度和下一个节点指针。
 */
typedef struct FieldNode {
    CString *fileName;       ///< 字段名
    int type;             ///< 字段类型
    int length;           ///< 字段长度
    struct FieldNode *next; ///< 下一个字段节点指针
} FieldNode;

/**
 * @brief Schema 结构体，用于表示数据库表的模式信息。
 */
typedef struct Schema {
    FieldNode *fields;        ///< 字段链表
    map_FileInfo_t *MapFileInfo; ///< 文件信息映射
} Schema;

/**
 * @brief 初始化一个新的 FileInfo 实例。
 *
 * @param type 文件类型。
 * @param length 文件长度。
 * @return 返回初始化后的 FileInfo 指针。
 */
FileInfo *FileInfoInit(int type, int length);

/**
 * @brief 初始化一个新的 Schema 实例。
 *
 * @return 返回初始化后的 Schema 指针。
 */
Schema *SchemaInit();

/**
 * @brief 向 Schema 中添加字段。
 *
 * @param schema 指向 Schema 实例的指针。
 * @param FldName 字段名。
 * @param type 字段类型。
 * @param length 字段长度。
 */
void SchemaAddField(Schema *schema, CString *FldName, int type, int length);

/**
 * @brief 向 Schema 中添加整数字段。
 *
 * @param schema 指向 Schema 实例的指针。
 * @param FldName 字段名。
 */
void SchemaAddIntField(Schema *schema, CString *FldName);

/**
 * @brief 向 Schema 中添加字符串字段。
 *
 * @param schema 指向 Schema 实例的指针。
 * @param FldName 字段名。
 * @param length 字符串长度。
 */
void SchemaAddStringField(Schema *schema, CString *FldName, int length);

/**
 * @brief 释放 Schema 实例资源。
 *
 * @param schema 指向要释放的 Schema 实例的指针。
 */
void SchemaFree(Schema *schema);

/**
 * @brief 获取指定字段的类型。
 *
 * @param schema 指向 Schema 实例的指针。
 * @param FldName 字段名。
 * @return 返回字段类型，对应 FileInfoCode 枚举。
 */
FileInfoCode SchemaType(Schema* schema, CString *FldName);

/**
 * @brief 获取指定字段的长度。
 *
 * @param schema 指向 Schema 实例的指针。
 * @param FldName 字段名。
 * @return 返回字段长度。
 */
int SchemaLength(Schema* schema, CString *FldName);

/**
 * @brief 从一个 Schema 向另一个 Schema 添加指定字段。
 *
 * @param SchemaTo 目标 Schema 指针。
 * @param FldName 字段名。
 * @param SchemaFrom 源 Schema 指针。
 */
void SchemaAdd(Schema *SchemaTo, CString *FldName, Schema *SchemaFrom);

/**
 * @brief 将一个 Schema 的所有字段添加到另一个 Schema。
 *
 * @param SchemaTo 目标 Schema 指针。
 * @param SchemaFrom 源 Schema 指针。
 */
void SchemaAddAll(Schema *SchemaTo, Schema *SchemaFrom);

/**
 * @brief 检查 Schema 是否包含指定字段。
 *
 * @param schema 指向 Schema 实例的指针。
 * @param FldName 字段名。
 * @return 如果包含该字段，返回 true；否则返回 false。
 */
bool SchemaHasField(Schema *schema, CString *FldName);
#endif //DBMS_C_SCHEMA_H