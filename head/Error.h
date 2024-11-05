//
// Created by yaohuayuan on 2024/10/21.
//

#ifndef DBMS_C_ERROR_H
#define DBMS_C_ERROR_H
#include "malloc.h"
#include "stdio.h"
typedef enum ErrorCode{
    Error_NULL,
    Error_HasSOtherLock,
    Error_HasXLock
}ErrorCode;
typedef struct Error{
    ErrorCode errorCode;
    char*  reason;
}Error;
Error * ErrorInit();
#endif //DBMS_C_ERROR_H
