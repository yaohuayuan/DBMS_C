//
// Created by yaohuayuan on 2024/11/19.
//

#ifndef DBMS_C_LEXER_H
#define DBMS_C_LEXER_H

#include "StreamTokenizer.h"
#include <stdbool.h>

#include "Term.h"

typedef struct Lexer {
    StreamTokenizer *tokenizer;
} Lexer;

Lexer* LexerInit(const char *s);
void LexerFree(Lexer *lexer);

// 匹配逻辑 (只看当前 Token，不移动指针)
bool LexerMatchDelim(Lexer *l, char d);
bool LexerMatchIntConstant(Lexer *l);
bool LexerMatchStringConstant(Lexer *l);
bool LexerMatchKeyWord(Lexer *l, const char *kw);
bool LexerMatchId(Lexer *l);

// 消费逻辑 (移动指针到下一个 Token)
void LexerEatDelim(Lexer *l, char d);
int LexerEatIntConstant(Lexer *l);
char* LexerEatStringConstant(Lexer *l);
void LexerEatKeyWord(Lexer *l, const char *kw);
char* LexerEatId(Lexer *l);
CompareOp LexerGetCurrentOp(Lexer *l);
void LexerNextToken(Lexer *l) ;
#endif