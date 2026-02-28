//
// Created by yaohuayuan on 2024/11/30.
//

#ifndef DBMS_C_PREDPARSER_H
#define DBMS_C_PREDPARSER_H
#include "Lexer.h"
/**
 * @brief PredParser 结构体定义，用于解析谓词。
 */
typedef struct PredParser{
    Lexer *lexer; ///< 指向词法分析器的指针
}PredParser;
#endif //DBMS_C_PREDPARSER_H
