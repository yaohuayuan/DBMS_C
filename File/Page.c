//
// Created by yaohuayuan on 2025/5/31.
//

#include "Page.h"
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
int PageGetInt(Page *p, int position){
    int output;
    bufferGetIntPosition(p->buffer,position,&output);
    return output;
}

void PageSetInt(Page*p ,int position,int data){
    bufferPutIntPosition(p->buffer,position,data);
}
void PageSetBytes(Page * p,int position,const uint8_t* data, uint64_t size){
    bufferPutIntPosition(p->buffer,position,size);
    bufferPutBytes(p->buffer,data,size);
}
uint8_t *PageGetBytes(Page * p,int position){
    int32_t length;
    p->buffer->position = position;
    bufferGetIntPosition(p->buffer,position,&length);
    uint8_t * data = malloc(length*sizeof (uint8_t));
    bufferGetBytes(p->buffer,data,length);
    return data;
}
CString * PageGetString(Page *p, int position) {
    p->buffer->position = position;
    int length = 0;
    bufferGetInt(p->buffer, &length);
    if (length <0) {
        return NULL;
    }
    char* str = (char*)malloc(length+1);
    bufferGetBytesPosition(p->buffer, position + sizeof(int32_t), str, length);
    str[length] = '\0';
    CString *cString = CStringCreateFromCStr(str);
    return cString;
}

void PageSetString(Page *p, int position, const CString * cs) {
    int length = (int)cs->length;
    p->buffer->position = position;
    bufferPutInt(p->buffer, length);
    bufferPutBytes(p->buffer, (uint8_t*)cs->data, length);
}

int PageMaxLength(CString *cString){
    return (int)cString->length+sizeof (int);
}
void PageDestroy(Page*p){
    bufferFree(p->buffer);
    free(p);
}
short PageGetShort(Page *p, int position){
    uint16_t data ;
    p->buffer->position = position;
    bufferGetShort(p->buffer,&data);
    return (short)data;

}
long long PageGetLong(Page *p, int position){
    uint64_t data ;
    p->buffer->position = position;
    bufferGetLong(p->buffer,&data);
    return (long long)data;
}

void PageSetBytesRaw(Page* p, int pos, const uint8_t* data, uint32_t size) {
    bufferPutBytesPosition(p->buffer, pos, data, size);
}

void PageGetBytesRaw(Page* p, int pos, uint8_t* out, uint32_t size) {
    bufferGetBytesPosition(p->buffer, pos, out, size);
}