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
char* PageGetString(Page *p,int position){
    ByteBufferData *data =ByteBufferDataInit();
    p->buffer->position = position;
    bufferGetInt(p->buffer, data);
    int length = *data->intData;
    ByteBufferData *s = ByteBufferDataInit();
    char* str = (char*)malloc(length + 1);
    bufferGetBytesPosition(p->buffer, position + sizeof(int32_t), s, length) ;
    if(s->StatusCode!=BYTEBUFFER_STRING){
        return NULL;
    }
    memmove(str,s->bytesData->data, length);
    str[length] = '\0';
    return str;
}
void PageSetString(Page * p,int position,const char* str){
    int32_t length = strlen(str);
    p->buffer->position = position;
    bufferPutInt(p->buffer,length);
    uint8_t *type = malloc(sizeof(uint8_t)*length);
    type[0] = BYTEBUFFER_STRING;
    bufferPutBytes(p->buffer, (uint8_t*)str,type, length);
}
int PageMaxLength(int strlen){
    return strlen+sizeof (int);
}