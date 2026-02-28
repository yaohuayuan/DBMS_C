//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_LEXER_H
#define DBMS_C_LEXER_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "Trie.h"
#include "StreamTokenizer.h"

// Lexer 结构体
typedef struct Lexer{
    StreamTokenizer *tokenizer;
    Trie *keyWords;
}Lexer;
/**
 * 初始化一个新的 Lexer 实例。
 *
 * @param s 要解析的字符串。
 * @return 返回初始化后的 Lexer 指针。
 */
Lexer* LexerInit(char *s);

/**
 * 匹配指定的分隔符。
 *
 * @param lexer 指向 Lexer 的指针。
 * @param d 要匹配的分隔符。
 * @return 如果匹配成功返回 true，否则返回 false。
 */
bool LexerMatchDelim(Lexer*lexer,char d);

/**
 * 匹配整数常量。
 *
 * @param lexer 指向 Lexer 的指针。
 * @return 如果匹配成功返回 true，否则返回 false。
 */
bool LexerMathIntConstant(Lexer *lexer);

/**
 * 匹配字符串常量。
 *
 * @param lexer 指向 Lexer 的指针。
 * @return 如果匹配成功返回 true，否则返回 false。
 */
bool LexerMatchStringConstant(Lexer*lexer);

/**
 * 匹配关键字。
 *
 * @param lexer 指向 Lexer 的指针。
 * @param word 要匹配的关键字。
 * @return 如果匹配成功返回 true，否则返回 false。
 */
bool LexerMatchKeyWord(Lexer *lexer,char *word);

/**
 * 匹配标识符。
 *
 * @param lexer 指向 Lexer 的指针。
 * @return 如果匹配成功返回 true，否则返回 false。
 */
bool LexerMatchId(Lexer*lexer);

/**
 * 跳过指定的分隔符。
 *
 * @param lexer 指向 Lexer 的指针。
 * @param ch 要跳过的分隔符。
 */
void LexerEatDelim(Lexer*lexer,char ch);

/**
 * 读取并返回整数常量。
 *
 * @param lexer 指向 Lexer 的指针。
 * @return 返回读取的整数值。
 */
int LexerEatIntConstant(Lexer *lexer);

/**
 * 读取并返回字符串常量。
 *
 * @param lexer 指向 Lexer 的指针。
 * @return 返回读取的字符串。
 */
char* LexerEatStringConstant(Lexer *lexer);

/**
 * 跳过指定的关键字。
 *
 * @param lexer 指向 Lexer 的指针。
 * @param s 要跳过的关键字。
 */
void LexerEatKeyWord(Lexer*lexer,char *s);

/**
 * 读取并返回标识符常量。
 *
 * @param lexer 指向 Lexer 的指针。
 * @return 返回读取的标识符字符串。
 */
char* LexerEatIDConstant(Lexer *lexer);

/**
 * 读取并返回标识符。
 *
 * @param lexer 指向 Lexer 的指针。
 * @return 返回读取的标识符字符串。
 */
char * LexerEatId(Lexer*lexer);
#endif //DBMS_C_LEXER_H
