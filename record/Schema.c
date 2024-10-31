#include "Schema.h"

// 初始化FileInfo
FileInfo *FileInfoInit(int type, int length) {
    FileInfo *fileInfo = malloc(sizeof(FileInfo));
    if (fileInfo) {
        fileInfo->type = type;
        fileInfo->length = length;
    }
    return fileInfo;
}

// 初始化Schema
Schema *SchemaInit() {
    Schema *schema = malloc(sizeof(Schema));
    if (!schema) {
        return NULL;
    }

    schema->fields = NULL;
    schema->MapFileInfo = malloc(sizeof(map_FileInfo_t));
    if (!schema->MapFileInfo) {
        free(schema);
        return NULL;
    }

    map_init(schema->MapFileInfo);
    return schema;
}

// 添加字段到Schema
void SchemaAddField(Schema *schema, char *FldName, int type, int length) {
    if (!schema || !FldName) {
        return;
    }
    FieldNode *fieldNode = schema->fields;

    // 新建字段节点
    FieldNode *newFieldNode = malloc(sizeof(FieldNode));
    if (!newFieldNode) {
        return;
    }
    newFieldNode->fileName = strdup(FldName);
    newFieldNode->type = type;
    newFieldNode->length = length;
    newFieldNode->next = NULL;

    // 如果是第一个字段
    if (!schema->fields) {
        schema->fields = newFieldNode;
    } else {
        // 找到最后一个字段节点并添加
        while (fieldNode->next) {
            fieldNode = fieldNode->next;
        }
        fieldNode->next = newFieldNode;
    }

    // 创建新FileInfo并添加到哈希映射
    FileInfo *fileInfo = FileInfoInit(type, length);
//    if (!fileInfo) {
//        free(newFieldNode->fileName);
//        free(newFieldNode);
//        return;
//    }
    map_set(schema->MapFileInfo, newFieldNode->fileName, *fileInfo);
//    // 使用&fileInfo而不是fileInfo
//    if (map_set(schema->MapFileInfo, newFieldNode->fileName, fileInfo) != 0) {
//        free(fileInfo);
//        free(newFieldNode->fileName);
//        free(newFieldNode);
//    }
}

// 添加整数字段
void SchemaAddIntField(Schema *schema, char *FldName) {
    SchemaAddField(schema, FldName, FILE_INFO_CODE_INTEGER, 0);
}

// 添加字符串字段
void SchemaAddStringField(Schema *schema, char *FldName, int length) {
    SchemaAddField(schema, FldName, FILE_INFO_CODE_VARCHAR, length);
}

// 释放Schema及其相关资源
void SchemaFree(Schema *schema) {
    if (!schema) {
        return;
    }
    FieldNode *current = schema->fields;
    while (current) {
        FieldNode *next = current->next;
        free(current->fileName);
        free(current);
        current = next;
    }

    // 释放哈希映射
    if (schema->MapFileInfo) {
        map_deinit(schema->MapFileInfo);
        free(schema->MapFileInfo);
    }

    free(schema);
}
