//
// Created by yaohuayuan on 2024/10/21.
//

#ifndef DBMS_C_ERROR_H
#define DBMS_C_ERROR_H

#include <malloc.h>
#include <stdio.h>

/**
 * @brief ErrorCode 枚举类型定义，用于标识错误代码。
 */
typedef enum ErrorCode {
    Error_NULL,              ///< 没有错误或空错误
    Error_HasSOtherLock,     ///< 已存在其他共享锁 (S 锁)
    Error_HasXLock           ///< 已存在排他锁 (X 锁)
} ErrorCode;

/**
 * @brief Error 结构体定义，用于封装错误信息。
 */
typedef struct Error {
    ErrorCode errorCode;     ///< 错误代码
    char* reason;            ///< 错误原因描述
} Error;

/**
 * @brief 初始化一个新的 Error 实例。
 *
 * @return 返回初始化后的 Error 指针。
 */
Error* ErrorInit();

#endif //DBMS_C_ERROR_H