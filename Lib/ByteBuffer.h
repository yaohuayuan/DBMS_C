//
// Created by yaohuayuan on 2025/5/31.
//

#ifndef NEWDBMS_BYTEBUFFER_H
#define NEWDBMS_BYTEBUFFER_H
#include <stdint.h>
#include <malloc.h>
#include <CString.h>
typedef enum {
    TYPE_INT,
    TYPE_BIGINT,
    TYPE_SMALLINT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_DECIMAL,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_VARCHAR,
    TYPE_TEXT,
    TYPE_DATE,
    TYPE_TIME,
    TYPE_TIMESTAMP,
    TYPE_BLOB,
    TYPE_JSON,
    TYPE_UUID,
    TYPE_NULL
} ByteBuffer_Type;
typedef struct ByteBuffer{
    uint64_t size;
    uint8_t* data;
    uint64_t position;
    uint64_t limit;
    uint8_t *type;
}ByteBuffer;
#endif //NEWDBMS_BYTEBUFFER_H
