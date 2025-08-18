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
Lexer* LexerInit(char *s);
bool LexerMatchDelim(Lexer*lexer,char d);
bool LexerMathIntConstant(Lexer *lexer);
bool LexerMatchStringConstant(Lexer*lexer);
bool LexerMatchKeyWord(Lexer *lexer,char *word);
bool LexerMatchId(Lexer*lexer);
void LexerEatDelim(Lexer*lexer,char ch);
int LexerEatIntConstant(Lexer *lexer);
char* LexerEatStringConstant(Lexer *lexer);
void LexerEatKeyWord(Lexer*lexer,char *s);
char* LexerEatIDConstant(Lexer *lexer);
char * LexerEatId(Lexer*lexer);
#endif //DBMS_C_LEXER_H
