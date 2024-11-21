//
// Created by yaohuayuan on 2024/9/18.
//

#include "Page.h"
Page *PageInit(int size){
    Page *p = malloc(sizeof(Page));
    p->buffer  = bufferAllocate(size);
    return p;
}
Page* PageInitBytes(ByteBuffer* buffer){
    Page *p = malloc(sizeof(Page));
    p->buffer = buffer;
    return p;
}
int PageGetInt(Page *p, int position, ByteBufferData *output){
    return bufferGetIntPosition(p->buffer,position,output);
}
ByteBuffer * PageContent(Page * p,int position){
    bufferPosition(p->buffer,position);
    return p->buffer;
}
void PageSetInt(Page*p ,int position,int data){
    bufferPutIntPosition(p->buffer,position,data);
}
void PageSetBytes(Page * p,int position,const uint8_t* data,uint8_t *type, uint64_t size){
    bufferPutIntPosition(p->buffer,position,size);
    bufferPutBytes(p->buffer,data,type,size);
}
ByteBufferData *PageGetBytes(Page * p,int position){
    ByteBufferData *byteBufferData = ByteBufferDataInit();
    int32_t length;
    p->buffer->position = position;
    bufferGetIntPosition(p->buffer,position,byteBufferData);
    length = *byteBufferData->intData;
    ByteBufferData *byteBufferData1 = ByteBufferDataInit();
    bufferGetBytes(p->buffer,byteBufferData1,length);
    return byteBufferData1;
}
char* PageGetString(Page *p, int position) {
    // 初始化用于获取数据的缓冲区数据结构
    ByteBufferData *data = ByteBufferDataInit();

    // 设置缓冲区的当前位置
    p->buffer->position = position;

    // 获取字符串长度
    bufferGetInt(p->buffer, data);
    int length = *data->intData;

    // 检查长度是否有效
    if (length <= 0) {
        return NULL; // 无效长度
    }

    // 初始化用于获取字节数据的结构
    ByteBufferData *s = ByteBufferDataInit();

    // 分配内存存储字符串
    char* str = (char*)malloc(length);

    // 从缓冲区读取字符串数据（包括 '\0'）
    bufferGetBytesPosition(p->buffer, position + sizeof(int32_t), s, length);

    // 检查类型是否为字符串类型
    if (s->StatusCode != BYTEBUFFER_STRING) {
        free(str);
        return NULL;
    }

    // 将数据复制到结果字符串
    memmove(str, s->bytesData->data, length);

    return str; // 返回完整的字符串（含 '\0'）
}

void PageSetString(Page *p, int position, const char* str) {
    // 计算字符串的长度（包括 '\0'）
    int32_t length = strlen(str) + 1; // 加 1 包含终止符

    // 设置缓冲区的当前位置
    p->buffer->position = position;

    // 存储字符串的长度
    bufferPutInt(p->buffer, length);

    // 为类型数据分配内存并设置为字符串类型
    uint8_t *type = malloc(sizeof(uint8_t) * length);
    type[0] = BYTEBUFFER_STRING;

    // 将字符串和类型信息存入缓冲区
    bufferPutBytes(p->buffer, (uint8_t*)str, type, length);

    // 释放分配的内存
    free(type);
}

int PageMaxLength(int strlen){
    return strlen+sizeof (int);
}