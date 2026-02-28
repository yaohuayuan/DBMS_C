//
// Created by yaohuayuan on 2024/11/20.
//

#ifndef DBMS_C_STREAMTOKENIZER_H
#define DBMS_C_STREAMTOKENIZER_H

#include "map.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief TokenType 枚举类型定义，用于标识令牌的类型。
 */
typedef enum {
    TOKEN_WORD,           ///< 单词
    TOKEN_INT_CONSTANT,   ///< 整数常量
    TOKEN_DELIM,          ///< 分隔符
    TOKEN_UNKNOWN,        ///< 未知类型
    TOKEN_NULL,           ///< 空格或空白字符
    TOKEN_END_OF          ///< 输入流结束标记
} DBTokenType;

/**
 * @brief StreamTokenizerTokenNode 结构体定义，用于存储单个令牌的信息。
 */
typedef struct StreamTokenizerTokenNode {
    int pos;              ///< 令牌在输入流中的位置
    DBTokenType type;       ///< 令牌的类型
    int len;              ///< 令牌的长度
} StreamTokenizerTokenNode;

/**
 * @brief StreamTokenizerTokenID 结构体定义，用于管理一组令牌信息。
 */
typedef struct StreamTokenizerTokenID {
    StreamTokenizerTokenNode *data; ///< 动态数组，存储令牌信息
    int len;                        ///< 当前令牌数量
    int endLen;                     ///< 当前处理到的位置
    int maxLen;                     ///< 数组的最大容量
} StreamTokenizerTokenID;

/**
 * @brief StreamTokenizer 结构体定义，用于解析输入流并生成令牌。
 */
typedef struct StreamTokenizer {
    char *data;                     ///< 输入数据字符串
    map_int_t *keySet;              ///< 存储关键字及类型映射的哈希表
    StreamTokenizerTokenID *streamTokenizerTokenId; ///< 令牌信息管理器
    char *sValue;                   ///< 当前解析出的字符串值
    int iValue;                     ///< 当前解析出的整数值
    DBTokenType type;                 ///< 当前令牌的类型
} StreamTokenizer;

/**
 * @brief 初始化一个新的 StreamTokenizer 实例。
 *
 * @param s 输入数据字符串。
 * @return 返回初始化后的 StreamTokenizer 指针。
 */
StreamTokenizer *StreamTokenizerInit(char *s);

/**
 * @brief 设置 StreamTokenizer 到小写模式，所有单词都将被转换为小写。
 *
 * @param streamTokenizer 指向 StreamTokenizer 的指针。
 */
void StreamTokenizerLowerCaseMode(StreamTokenizer *streamTokenizer);

/**
 * @brief 将指定字符设置为普通字符（非分隔符）。
 *
 * @param streamTokenizer 指向 StreamTokenizer 的指针。
 * @param ch 要设置的字符。
 */
void StreamTokenizerOrdinaryChar(StreamTokenizer *streamTokenizer, char ch);

/**
 * @brief 初始化一个新的 StreamTokenizerTokenID 实例。
 *
 * @return 返回初始化后的 StreamTokenizerTokenID 指针。
 */
StreamTokenizerTokenID *StreamTokenizerTokenIdInit();

/**
 * @brief 添加范围内的字符作为单词字符到 keySet 中。
 *
 * @param streamTokenizer 指向 StreamTokenizer 的指针。
 * @param start 字符范围的起始字符。
 * @param end 字符范围的结束字符。
 */
void StreamTokenizerWordChars(StreamTokenizer *streamTokenizer, char start, char end);

/**
 * @brief 设置当前解析出的令牌类型。
 *
 * @param streamTokenizer 指向 StreamTokenizer 的指针。
 */
void StreamTokenizerType(StreamTokenizer* streamTokenizer);

/**
 * @brief 准备 StreamTokenizer 进行下一次搜索。
 *
 * @param streamTokenizer 指向 StreamTokenizer 的指针。
 */
void StreamTokenizerSearchReady(StreamTokenizer *streamTokenizer);

/**
 * @brief 解析下一个令牌。
 *
 * @param streamTokenizer 指向 StreamTokenizer 的指针。
 * @return 返回令牌类型：
 *         - 0: TOKEN_END_OF (EOF)
 *         - -3: TOKEN_WORD
 *         - -2: TOKEN_INT_CONSTANT
 *         - 其他: TOKEN_DELIM (返回分隔符字符)
 */
int StreamTokenizerNext(StreamTokenizer *streamTokenizer);

#endif //DBMS_C_STREAMTOKENIZER_H