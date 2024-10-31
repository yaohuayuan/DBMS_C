//
// Created by yaohuayuan on 2024/9/18.
//

#ifndef DBMS_C_PAGE_H
#define DBMS_C_PAGE_H
#include"../head/ByteBuffer.h"
#include<stdio.h>
typedef struct{
    ByteBuffer *buffer;
}Page;
Page *PageInit(int size);
Page* PageInitBytes(ByteBuffer* buffer);
int PageGetInt(Page *p, int position, ByteBufferData *output);
void PageSetInt(Page*p ,int position,int data);
int PageGetShot(Page *p,int position);
int PageGetLong(Page *p,int position);
char* PageGetString(Page *p,int position);
void PageSetString(Page * p,int position,const char* str);
ByteBuffer * PageContent(Page * p,int position);
void PageSetBytes(Page * p,int position,const uint8_t* data,uint8_t *type, uint64_t size);
ByteBufferData *PageGetBytes(Page * p,int position);
int PageMaxLength(int strlen);
#endif //DBMS_C_PAGE_H
