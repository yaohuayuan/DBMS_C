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

    schema->fields = NULL; // 保持为NULL，因为添加第一个字段时会设置
    schema->MapFileInfo = malloc(sizeof(map_FileInfo_t));
    if (!schema->MapFileInfo) {
        free(schema);
        return NULL;
    }

    map_init(schema->MapFileInfo);
    return schema;
}

// 添加字段到Schema
void SchemaAddField(Schema *schema, CString *FldName, int type, int length) {
    if (!schema || !FldName) {
        return;
    }
    FieldNode *fieldNode = schema->fields;

    // 新建字段节点
    FieldNode *newFieldNode = malloc(sizeof(FieldNode));
    if (!newFieldNode) {
        return;
    }
    newFieldNode->fileName = CStringCreateFromCString(FldName);
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
    if (!fileInfo) {
        CStringDestroy(newFieldNode->fileName);
        free(newFieldNode);
        return;
    }
    // 使用CStringGetPtr获取C风格字符串指针
    if (map_set(schema->MapFileInfo, CStringGetPtr(newFieldNode->fileName), *fileInfo) != 0) {
        free(fileInfo);
        CStringDestroy(newFieldNode->fileName);
        free(newFieldNode);
    }
}

// 添加整数字段
void SchemaAddIntField(Schema *schema, CString *FldName) {
    SchemaAddField(schema, FldName, FILE_INFO_CODE_INTEGER, 4);
}

// 添加字符串字段
void SchemaAddStringField(Schema *schema, CString *FldName, int length) {
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
        CStringDestroy(current->fileName);
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
FileInfoCode SchemaType(Schema* schema, CString *FldName){
    FileInfo *fileInfo= (FileInfo*)map_get(schema->MapFileInfo, CStringGetPtr(FldName));
    return fileInfo->type;
}
int SchemaLength(Schema* schema, CString *FldName){
    FileInfo *fileInfo= (FileInfo*)map_get(schema->MapFileInfo, CStringGetPtr(FldName));
    return fileInfo->length;
}
void SchemaAdd(Schema *SchemaTo, CString *FldName, Schema *SchemaFrom){
    int type = SchemaType(SchemaFrom, FldName);
    int length = SchemaLength(SchemaFrom, FldName);
    SchemaAddField(SchemaTo, FldName, type, length);
}
void SchemaAddAll(Schema *SchemaTo, Schema *SchemaFrom){
    FieldNode* fieldNode = SchemaFrom->fields;
    while(fieldNode!=NULL){
        SchemaAdd(SchemaTo, fieldNode->fileName, SchemaFrom);
        fieldNode = fieldNode->next;
    }
}
bool SchemaHasField(Schema *schema, CString *FldName){
    FieldNode* fieldNode = schema->fields;
    while(fieldNode!=NULL){
        if(CStringCompare(FldName, fieldNode->fileName) == 0)
            return true;
        fieldNode = fieldNode->next;
    }
    return false;
}