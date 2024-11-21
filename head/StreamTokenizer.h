//
// Created by yaohuayuan on 2024/11/20.
//

#ifndef DBMS_C_STREAMTOKENIZER_H
#define DBMS_C_STREAMTOKENIZER_H
#include "map.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"

// TokenType 枚举类型定义
typedef enum {
    TOKEN_WORD,           // 单词
    TOKEN_INT_CONSTANT,   // 整数常量
    TOKEN_DELIM,          // 分隔符
    TOKEN_UNKNOWN  ,       // 未知类型
    TOKEN_NULL,            // 空格
    TOKEN_END_OF,
} TokenType;

// StreamTokenizerTokenNode 结构体
typedef struct StreamTokenizerTokenNode {
    int pos;              // 令牌的位置
    TokenType type;       // 令牌的类型
    int len;
} StreamTokenizerTokenNode;

// StreamTokenizerTokenID 结构体
typedef struct StreamTokenizerTokenID {
    StreamTokenizerTokenNode *data; // 动态数组存储令牌信息
    int len;                        // 当前令牌数量
    int endLen;
    int maxLen;                     // 最大容量
} StreamTokenizerTokenID;

// StreamTokenizer 结构体
typedef struct StreamTokenizer {
    char *data;                     // 输入数据
    map_int_t *keySet;              // 存储关键字及类型映射
    StreamTokenizerTokenID *streamTokenizerTokenId; // 令牌信息
    char *sValue;                   // 字符串值
    double iValue;                  // 数值值
    TokenType type;                 // 当前令牌类型
} StreamTokenizer;

// 函数声明
StreamTokenizer *StreamTokenizerInit(char *s);
void StreamTokenizerLowerCaseMode(StreamTokenizer *streamTokenizer);
void StreamTokenizerOrdinaryChar(StreamTokenizer *streamTokenizer, char ch) ;
StreamTokenizerTokenID * StreamTokenizerTokenIdInit();
// 添加单词字符到 keySet
void StreamTokenizerWordChars(StreamTokenizer *streamTokenizer, char start, char end) ;
void StreamTokenizerType(StreamTokenizer*streamTokenizer);
void StreamTokenizerSearchReady(StreamTokenizer *streamTokenizer);
void StreamTokenizerNext(StreamTokenizer *streamTokenizer);
#endif //DBMS_C_STREAMTOKENIZER_H
