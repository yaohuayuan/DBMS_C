//
// Created by yaohuayuan on 2024/11/20.
//

#ifndef DBMS_C_STREAMTOKENIZER_H
#define DBMS_C_STREAMTOKENIZER_H

#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Token 类型枚举 (STT_DELIM 建议在 Lexer 中进一步处理符号细节)
 */
typedef enum {
    STT_EOF = 0,
    STT_WORD,       // 标识符/关键字 (SELECT, name)
    STT_NUMBER,     // 整数 (123, 99999999)
    STT_STRING,     // 字符串 ('Alice')
    STT_DELIM,      // 运算符和符号 (>=, <=, !=, <>, =, ,, ;)
    STT_ERROR       // 错误 (未闭合的引号等)
} StreamTokenType;

/**
 * @brief Tokenizer 结构体
 */
typedef struct {
    const char *input;      // 原始输入
    const char *pos;        // 当前游标
    const char *start;      // 当前 Token 起始指针
    int length;             // 当前 Token 长度
    long long intValue;     // 升级为 long long 防止溢出
    StreamTokenType type;   // 当前 Token 类型
} StreamTokenizer;

// --- API ---
StreamTokenizer* StreamTokenizerInit(const char *input);
int StreamTokenizerNext(StreamTokenizer *st);
void StreamTokenizerFree(StreamTokenizer *st);

#define StreamTokenizerGetText(st) ((st)->start)
#define StreamTokenizerGetLength(st) ((st)->length)
#define StreamTokenizerGetInt(st) ((st)->intValue)

#endif //DBMS_C_STREAMTOKENIZER_H