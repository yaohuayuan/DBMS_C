
#ifndef DBMS_C_SCHEMA_H
#define DBMS_C_SCHEMA_H

#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "map.h"
#include "stdbool.h"
// 文件信息类型枚举
typedef enum {
    FILE_INFO_CODE_BIT,
    FILE_INFO_CODE_TINYINT,
    FILE_INFO_CODE_SMALLINT,
    FILE_INFO_CODE_INTEGER,
    FILE_INFO_CODE_BIGINT,
    FILE_INFO_CODE_FLOAT,
    FILE_INFO_CODE_REAL,
    FILE_INFO_CODE_DOUBLE,
    FILE_INFO_CODE_NUMERIC,
    FILE_INFO_CODE_DECIMAL,
    FILE_INFO_CODE_CHAR,
    FILE_INFO_CODE_VARCHAR,
    FILE_INFO_CODE_LONG_VARCHAR,
    FILE_INFO_CODE_DATE,
    FILE_INFO_CODE_TIME,
    FILE_INFO_CODE_TIMESTAMP,
    FILE_INFO_CODE_BINARY,
    FILE_INFO_CODE_OTHER,
    FILE_INFO_CODE_NULL,
} FileInfoCode;

// 文件信息结构体
typedef struct FileInfo {
    int type;
    int length;
} FileInfo;
typedef map_t(FileInfo) map_FileInfo_t;
// 字段节点结构体
typedef struct FieldNode {
    char *fileName;
    int type;
    int length;
    struct FieldNode *next;
} FieldNode;

// 模式结构体
typedef struct Schema {
    FieldNode *fields;
    map_FileInfo_t *MapFileInfo;
} Schema;
FileInfo *FileInfoInit(int type, int length);
Schema *SchemaInit();
void SchemaAddField(Schema *schema, char *FldName, int type, int length);
void SchemaAddIntField(Schema *schema, char *FldName);
void SchemaAddStringField(Schema *schema, char *FldName, int length);
void SchemaFree(Schema *schema);
FileInfoCode SchemaType(Schema* schema,char* FldName);
int SchemaLength(Schema* schema,char* FldName);
void SchemaAdd(Schema *SchemaTo,char* FldName,Schema *SchemaFrom);
void SchemaAddAll(Schema *SchemaTo,Schema *SchemaFrom);
bool SchemaHasField(Schema *schema,char* FldName);
#endif //DBMS_C_SCHEMA_H