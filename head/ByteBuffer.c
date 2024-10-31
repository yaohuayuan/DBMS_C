//
// Created by yaohuayuan on 2024/9/18.
//

#include "ByteBuffer.h"
ByteBufferData* ByteBufferDataInit() {
    ByteBufferData *buffer = (ByteBufferData *)malloc(sizeof(ByteBufferData));
    if (buffer == NULL)
        return NULL;
    buffer->StatusCode = BYTEBUFFER_INIT;
    buffer->intData = (int *)malloc(sizeof(int));
    if (buffer->intData != NULL)
        *buffer->intData = 0;
    buffer->shortData = (short *)malloc(sizeof(short));
    if (buffer->shortData != NULL)
        *buffer->shortData = 0;
    buffer->longData = (long long *)malloc(sizeof(long long));
    if (buffer->longData != NULL)
        *buffer->longData = 0;
    buffer->stringData = (StringData *)malloc(sizeof(StringData));
    if (buffer->stringData != NULL) {
        buffer->stringData->length = 0;
        buffer->stringData->data = NULL;
    }
    buffer->byteData = (uint8_t *)malloc(1);
    if (buffer->byteData != NULL)
        *buffer->byteData = 0;
    buffer->bytesData = (BytesData*) malloc(sizeof(BytesData));
    if(buffer->bytesData!=NULL){
        buffer->bytesData->data=NULL;
        buffer->bytesData->type = NULL;
        buffer->bytesData->length=0;
    }
    return buffer;
}

ByteBuffer* bufferAllocate(uint64_t size){
    ByteBuffer *buffer = malloc(sizeof(ByteBuffer));
    if(buffer == NULL)
        return NULL;
    buffer->size = size;
    buffer->data = malloc(sizeof(uint8_t)*size);
    buffer->type = malloc(sizeof(uint8_t)*size);
    if(buffer->data == NULL)
        return NULL;
    bufferClear(buffer);
    return buffer;
}
void bufferClear(ByteBuffer* buffer){
    memset(buffer->data,0,buffer->size);
    memset(buffer->type,BYTEBUFFER_INIT,buffer->size);
    buffer->position = 0;
    buffer->limit = buffer->size;
}
void bufferFree(ByteBuffer* buffer){
    free(buffer->data);
    free(buffer->type);
    free(buffer);
}
void bufferCompact(ByteBuffer* buffer){
    uint64_t numberOfBytes = buffer->limit - buffer->position;
    memmove(buffer->data,&(buffer->data[buffer->position]),numberOfBytes);
    buffer->limit = buffer->size;
    buffer->position = numberOfBytes;
}
void bufferFlip(ByteBuffer* buffer){
    buffer->limit = buffer->position;
    buffer->position = 0;
}
uint8_t bufferPut(ByteBuffer* buffer, uint8_t data){
    if(buffer == NULL){
        return 0;
    }
    if(buffer->position >= buffer->size){
        return 0;
    }
    buffer->data[buffer->position++]=data;
    return 1;
}
uint8_t bufferPutShort(ByteBuffer* buffer, int16_t data){
    if(buffer == NULL)
        return 0;
    if(buffer->position+1 >= buffer->size)
        return 0;
    buffer->type[buffer->position] = BYTEBUFFER_SHORT;
    buffer->data[buffer->position++] = data & 255;
    buffer->data[buffer->position++] = (data >> 8) & 255;
    return 2;
}
uint8_t bufferPutInt(ByteBuffer* buffer, int32_t data){
    if(buffer == NULL)
        return 0;
    if(buffer->position + 3 >= buffer->size){
        return 0;
    }
    buffer->type[buffer->position] = BYTEBUFFER_INT;
    buffer->data[buffer->position++] = data & 255;
    buffer->data[buffer->position++] = (data >> 8) & 255;
    buffer->data[buffer->position++] = (data >> 16) & 255;
    buffer->data[buffer->position++] = (data >> 24) & 255;
    return 4;
}
uint8_t bufferPutLong(ByteBuffer* buffer, int64_t data){
    if(buffer == NULL)
        return 0;
    if(buffer->position + 7 > buffer->size)
        return 0;
    buffer->type[buffer->position] = BYTEBUFFER_LONG;
    buffer->data[buffer->position++] = data & 255;
    buffer->data[buffer->position++] = (data >> 8) & 255;
    buffer->data[buffer->position++] = (data >> 16) & 255;
    buffer->data[buffer->position++] = (data >> 24) & 255;
    buffer->data[buffer->position++] = (data >> 32) & 255;
    buffer->data[buffer->position++] = (data >> 40) & 255;
    buffer->data[buffer->position++] = (data >> 48) & 255;
    buffer->data[buffer->position++] = (data >> 56) & 255;
    return 8;
}
uint8_t bufferGet(ByteBuffer* buffer, ByteBufferData *data){
    if(buffer == NULL)
        return 0;
    if(buffer->position >= buffer->limit)
        return 0;
    data->StatusCode = BYTEBUFFER_BYTE;
    *data->byteData = buffer->data[buffer->position++];
    return 1;
}
uint8_t bufferGetShort(ByteBuffer* buffer, ByteBufferData *data){
    if(buffer == NULL)
        return 0;
    if(buffer->position + 1 > buffer->limit)
        return 0;
    if(buffer->type[buffer->position]!=BYTEBUFFER_SHORT&&buffer->type[buffer->position]!=BYTEBUFFER_BYTES){
        return 0;
    }
    data->StatusCode = BYTEBUFFER_SHORT;
    *data->shortData = *((int16_t*)(buffer->data + buffer->position));
    buffer->position += 2;
    return 2;
}
uint8_t bufferGetInt(ByteBuffer* buffer, ByteBufferData *data){
    if(buffer == NULL)
        return 0;
    if(buffer->position + 3 > buffer->limit)
        return 0;
    if(buffer->type[buffer->position]!=BYTEBUFFER_INT&&buffer->type[buffer->position]!=BYTEBUFFER_BYTES)
        return 0;
    data->StatusCode = BYTEBUFFER_INT;
    *data->intData = *((int32_t*)(buffer->data + buffer->position));
    buffer->position += 4;
    return 4;
}
uint8_t bufferGetLong(ByteBuffer* buffer, ByteBufferData *data){
    if(buffer == NULL)
        return 0;
    if(buffer->position + 7 > buffer->limit)
        return 0;
    if(buffer->type[buffer->position]!=BYTEBUFFER_LONG&&buffer->type[buffer->position]!=BYTEBUFFER_BYTES)
        return 0;
    data->StatusCode = BYTEBUFFER_LONG;
    *data->longData = *((int64_t*)(buffer->data + buffer->position));
    buffer->position += 8;
    return 8;
}
uint64_t bufferGetBytes(ByteBuffer* buffer, ByteBufferData *data, int size){
    if(buffer == NULL)
        return 0;
    if(buffer->position + size > buffer->limit)
        return 0;
    data->StatusCode = buffer->type[buffer->position];
    data->bytesData->data = malloc(sizeof(uint8_t)*size);
    data->bytesData->type = malloc(sizeof(uint8_t)*size);
    memmove(data->bytesData->data, buffer->data + buffer->position, size);
    memmove(data->bytesData->type, buffer->type + buffer->position, size);
    data->bytesData->length=size;
    return size;
}
uint8_t bufferPutPosition(ByteBuffer* buffer, uint64_t position,uint8_t data){
    buffer->position = position;
    return bufferPut(buffer,data);
}
uint8_t bufferPutShortPosition(ByteBuffer* buffer, uint64_t position, int16_t data){
    buffer->position = position;
    return bufferPutShort(buffer,data);
}
uint8_t bufferPutIntPosition(ByteBuffer* buffer, uint64_t position, int32_t data){
    buffer->position = position;
    return bufferPutInt(buffer,data);
}
uint8_t bufferPutLongPosition(ByteBuffer* buffer, uint64_t position, int64_t data){
    buffer->position = position;
    return bufferPutLong(buffer,data);
}
uint8_t bufferGetPosition(ByteBuffer* buffer, uint64_t position, ByteBufferData *output){
    buffer->position = position;
    return bufferGet(buffer,output);
}
uint8_t bufferGetShortPosition(ByteBuffer* buffer, uint64_t position, ByteBufferData *output){
    buffer->position = position;
    return bufferGetShort(buffer,output);
}
uint8_t bufferGetIntPosition(ByteBuffer* buffer, uint64_t position, ByteBufferData* output){
    buffer->position = position;
    return bufferGetInt(buffer,  output);
}
uint8_t bufferGetLongPosition(ByteBuffer* buffer, uint64_t position, ByteBufferData* output){
    buffer->position = position;
    return bufferGetLong(buffer,output);
}
uint64_t bufferGetBytesPosition(ByteBuffer* buffer,uint64_t position,ByteBufferData *data, int size){
    buffer->position = position;
    return bufferGetBytes(buffer,data,size);
}
uint64_t bufferPosition(ByteBuffer* buffer, uint64_t newPosition) {
    if (newPosition >= buffer->size) {
        return 0;
    }
    buffer->position = newPosition;
    return buffer->position;
}
uint64_t bufferPutBytesPosition(ByteBuffer* buffer, uint64_t position, uint8_t* data,uint8_t *type, uint64_t size) {
    // 检查缓冲区是否有效以及写入是否越界
    if (buffer == NULL || data == NULL || position + size > buffer->limit) {
        return 0;
    }
    memcpy(buffer->data + position, data, size);
    memcpy(buffer->type + position, type, size);
    return size;
}
uint64_t bufferPutBytes(ByteBuffer* buffer, uint8_t* data,uint8_t *type,uint64_t size) {
    if (buffer == NULL || data == NULL || buffer->position + size > buffer->limit) {
        return 0;
    }
    memcpy(buffer->data + buffer->position, data, size);
    memcpy(buffer->type + buffer->position, type, size);
    buffer->position += size;
    return size;
}
