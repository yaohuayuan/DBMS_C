//
// Created by yaohuayuan on 2025/5/31.
//

#include "ByteBuffer.h"
#define CHECK_BUFFER(b) do { if (!(b)) return BYTEBUFFER_ERROR_NULL; } while (0)
#define CHECK_BOUNDS(b, s) do { \
    if ((b)->position + (s) > (b)->limit) return BYTEBUFFER_ERROR_BOUNDS; \
} while (0)
ByteBuffer* bufferAllocate(uint64_t size){
    if(size<=0)
        return NULL;
    ByteBuffer *buffer = malloc(sizeof(ByteBuffer));
    if(buffer == NULL)
        return NULL;
    buffer->size = size;
    buffer->data = malloc(sizeof(uint8_t)*size);
    if(buffer->data == NULL)
        return NULL;
    bufferClear(buffer);
    return buffer;
}
ByteBuffer *bufferInitByteSize(uint64_t size,uint8_t*data){
    if(size<=0)
        return NULL;
    ByteBuffer *buffer = malloc(sizeof(ByteBuffer));
    if(buffer == NULL)
        return NULL;
    buffer->size = size;
    buffer->data = malloc(sizeof(uint8_t)*size);
    if(buffer->data == NULL)
        return NULL;
    bufferClear(buffer);
    memcpy(buffer->data,data,size);
    return buffer;
}
void bufferClear(ByteBuffer* buffer){    if (!buffer) return;    memset(buffer->data,0,buffer->size);    buffer->position = 0;    buffer->limit = buffer->size;}
void bufferFree(ByteBuffer* buffer){
    free(buffer->data);
    free(buffer);
}
void bufferCompact(ByteBuffer* buffer){    if (!buffer) return;    uint64_t numberOfBytes = buffer->limit - buffer->position;    memmove(buffer->data,&(buffer->data[buffer->position]),numberOfBytes);    buffer->limit = buffer->size;    buffer->position = numberOfBytes;}
void bufferFlip(ByteBuffer* buffer){    if (!buffer) return;    buffer->limit = buffer->position;    buffer->position = 0;}
ByteBufferError bufferPut(ByteBuffer* buffer, uint8_t data){
    if (!buffer || !buffer->data) return BYTEBUFFER_ERROR_NULL;
    if (buffer->position + 1 > buffer->limit) return BYTEBUFFER_ERROR_BOUNDS;
    buffer->data[buffer->position++]=data;
    return BYTEBUFFER_OK;
}
ByteBufferError bufferPutShort(ByteBuffer* buffer, int16_t data){
    if (!buffer || !buffer->data) return BYTEBUFFER_ERROR_NULL;
    if (buffer->position + 2 > buffer->limit) return BYTEBUFFER_ERROR_BOUNDS;
    buffer->data[buffer->position++] = data & 255;
    buffer->data[buffer->position++] = (data >> 8) & 255;
    return BYTEBUFFER_OK;
}
ByteBufferError bufferPutInt(ByteBuffer* buffer, int32_t data){
    if (!buffer || !buffer->data) return BYTEBUFFER_ERROR_NULL;
    if (buffer->position + 4 > buffer->limit) return BYTEBUFFER_ERROR_BOUNDS;
    buffer->data[buffer->position++] = data & 255;
    buffer->data[buffer->position++] = (data >> 8) & 255;
    buffer->data[buffer->position++] = (data >> 16) & 255;
    buffer->data[buffer->position++] = (data >> 24) & 255;
    return BYTEBUFFER_OK;
}
ByteBufferError bufferPutLong(ByteBuffer* buffer, int64_t data){
    if (!buffer || !buffer->data) return BYTEBUFFER_ERROR_NULL;
    if (buffer->position + 8 > buffer->limit) return BYTEBUFFER_ERROR_BOUNDS;
    buffer->data[buffer->position++] = data & 255;
    buffer->data[buffer->position++] = (data >> 8) & 255;
    buffer->data[buffer->position++] = (data >> 16) & 255;
    buffer->data[buffer->position++] = (data >> 24) & 255;
    buffer->data[buffer->position++] = (data >> 32) & 255;
    buffer->data[buffer->position++] = (data >> 40) & 255;
    buffer->data[buffer->position++] = (data >> 48) & 255;
    buffer->data[buffer->position++] = (data >> 56) & 255;
    return BYTEBUFFER_OK;
}
ByteBufferError bufferGet(ByteBuffer* buffer, uint8_t *data){
    if (!buffer || !buffer->data || !data) return BYTEBUFFER_ERROR_NULL;
    if (buffer->position + 1 > buffer->limit) return BYTEBUFFER_ERROR_BOUNDS;
    *data = (uint8_t)(buffer->data[buffer->position]);
    buffer->position++;
    return BYTEBUFFER_OK;
}
ByteBufferError bufferGetShort(ByteBuffer* buffer, uint16_t *data){
    if (!buffer || !buffer->data || !data) return BYTEBUFFER_ERROR_NULL;
    if (buffer->position + 2 > buffer->limit) return BYTEBUFFER_ERROR_BOUNDS;
    *data = (uint16_t)buffer->data[buffer->position] |
            ((uint16_t)buffer->data[buffer->position + 1] << 8);
    buffer->position += 2;
    return BYTEBUFFER_OK;
}
ByteBufferError bufferGetInt(ByteBuffer* buffer, int32_t *data){
    if (!buffer || !buffer->data || !data) return BYTEBUFFER_ERROR_NULL;
    if (buffer->position + 4 > buffer->limit) return BYTEBUFFER_ERROR_BOUNDS;
    *data = (int32_t)buffer->data[buffer->position] |
            ((int32_t)buffer->data[buffer->position+1] << 8) |
            ((int32_t)buffer->data[buffer->position+2] << 16) |
            ((int32_t)buffer->data[buffer->position+3] << 24);

    buffer->position += 4;
    return BYTEBUFFER_OK;
}
ByteBufferError bufferGetLong(ByteBuffer* buffer, uint64_t *data){
    if (!buffer || !buffer->data || !data) return BYTEBUFFER_ERROR_NULL;
    if (buffer->position + 8 > buffer->limit) return BYTEBUFFER_ERROR_BOUNDS;
    *data =  (int64_t)buffer->data[buffer->position] |
             ((int64_t)buffer->data[buffer->position + 1] << 8) |
             ((int64_t)buffer->data[buffer->position + 2] << 16) |
             ((int64_t)buffer->data[buffer->position + 3] << 24) |
             ((int64_t)buffer->data[buffer->position + 4] << 32) |
             ((int64_t)buffer->data[buffer->position + 5] << 40) |
             ((int64_t)buffer->data[buffer->position + 6] << 48) |
             ((int64_t)buffer->data[buffer->position + 7] << 56);
    buffer->position += 8;
    return BYTEBUFFER_OK;
}
ByteBufferError bufferGetBytes(ByteBuffer* buffer, uint8_t *data, int size){
    if (!buffer || !buffer->data || !data) return BYTEBUFFER_ERROR_NULL;
    if (buffer->position + size > buffer->limit) return BYTEBUFFER_ERROR_BOUNDS;
    memmove(data, buffer->data + buffer->position, size);
    buffer->position += size;
    return BYTEBUFFER_OK;
}
ByteBufferError bufferPutPosition(ByteBuffer* buffer, uint64_t position,uint8_t data){
    if (!buffer) return BYTEBUFFER_ERROR_NULL;
    buffer->position = position;
    return bufferPut(buffer,data);
}
ByteBufferError bufferPutShortPosition(ByteBuffer* buffer, uint64_t position, int16_t data){
    if (!buffer) return BYTEBUFFER_ERROR_NULL;
    buffer->position = position;
    return bufferPutShort(buffer,data);
}
ByteBufferError bufferPutIntPosition(ByteBuffer* buffer, uint64_t position, int32_t data){
    if (!buffer) return BYTEBUFFER_ERROR_NULL;
    buffer->position = position;
    return bufferPutInt(buffer,data);
}
ByteBufferError bufferPutLongPosition(ByteBuffer* buffer, uint64_t position, int64_t data){
    if (!buffer) return BYTEBUFFER_ERROR_NULL;
    buffer->position = position;
    return bufferPutLong(buffer,data);
}
ByteBufferError bufferGetPosition(ByteBuffer* buffer, uint64_t position, uint8_t *output){
    if (!buffer) return BYTEBUFFER_ERROR_NULL;
    buffer->position = position;
    return bufferGet(buffer,output);
}
ByteBufferError bufferGetShortPosition(ByteBuffer* buffer, uint64_t position, uint16_t *output){
    if (!buffer) return BYTEBUFFER_ERROR_NULL;
    buffer->position = position;
    return bufferGetShort(buffer,output);
}
ByteBufferError bufferGetIntPosition(ByteBuffer* buffer, uint64_t position, int32_t * output){
    if (!buffer) return BYTEBUFFER_ERROR_NULL;
    buffer->position = position;
    return bufferGetInt(buffer,  output);
}
ByteBufferError bufferGetLongPosition(ByteBuffer* buffer, uint64_t position, uint64_t * output){
    if (!buffer) return BYTEBUFFER_ERROR_NULL;
    buffer->position = position;
    return bufferGetLong(buffer,output);
}
ByteBufferError bufferGetBytesPosition(ByteBuffer* buffer,uint64_t position,uint8_t *data, int size){
    if (!buffer) return BYTEBUFFER_ERROR_NULL;
    buffer->position = position;
    return bufferGetBytes(buffer,data,size);
}
uint64_t bufferPosition(ByteBuffer* buffer, uint64_t newPosition) {
    if (!buffer) return 0;
    if (newPosition >= buffer->size) {
        return 0;
    }
    buffer->position = newPosition;
    return newPosition;
}
ByteBufferError bufferPutBytesPosition(ByteBuffer* buffer, uint64_t position, uint8_t* data, uint64_t size) {
    // 检查缓冲区是否有效以及写入是否越界
    if (!buffer || !buffer->data || !data) return BYTEBUFFER_ERROR_NULL;
    if (position + size > buffer->limit){
        return BYTEBUFFER_ERROR_BOUNDS;
    }

    memcpy(buffer->data + position, data, size);
    return BYTEBUFFER_OK;
}
ByteBufferError bufferPutBytes(ByteBuffer* buffer, uint8_t* data,uint64_t size) {
    if (!buffer || !buffer->data || !data) return BYTEBUFFER_ERROR_NULL;
    if (buffer->position + size > buffer->limit) return BYTEBUFFER_ERROR_BOUNDS;
    memcpy(buffer->data + buffer->position, data, size);
    buffer->position += size;
    return BYTEBUFFER_OK;
}
