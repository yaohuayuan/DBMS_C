//
// Created by yaohuayuan on 2024/11/19.
//

#include "Lexer.h"
#include "string.h"
Lexer* LexerInit(char *s){
    char *LexerWords[]={
            "select", "from", "where", "and",
            "insert", "into", "values", "delete", "update", "set",
            "create", "table", "int", "varchar", "view", "as", "index", "on"
    };
    Lexer *lexer = malloc(sizeof (Lexer));
    lexer->tokenizer = StreamTokenizerInit(s);
    StreamTokenizerLowerCaseMode(lexer->tokenizer);
    StreamTokenizerOrdinaryChar(lexer->tokenizer,'.');
    StreamTokenizerWordChars(lexer->tokenizer,'_','_');
    lexer->keyWords = TrieInit();
    int wordCount = sizeof(LexerWords) / sizeof(LexerWords[0]);
    for(int i=0;i<wordCount;i++){
        TrieInsert(lexer->keyWords,LexerWords[i]);
    }
    StreamTokenizerSearchReady(lexer->tokenizer);
    return lexer;
}
bool LexerMatchDelim(Lexer*lexer,char d){
    char key[2] = {d,'\0'};
    return lexer->tokenizer->type==TOKEN_DELIM&&strcmp(lexer->tokenizer->sValue,key) == 0;
}
bool LexerMathIntConstant(Lexer *lexer){
    return lexer->tokenizer->type==TOKEN_INT_CONSTANT;
}
bool LexerMatchStringConstant(Lexer*lexer){
    return lexer->tokenizer->type==TOKEN_WORD&&lexer->tokenizer->sValue[0]=='\'';
}
bool LexerMatchKeyWord(Lexer *lexer,char *word){
    return lexer->tokenizer->type == TOKEN_WORD&& strcmp(lexer->tokenizer->sValue,word)==0;
}
bool LexerMatchId(Lexer*lexer){
    return lexer->tokenizer->type == TOKEN_WORD&&lexer->tokenizer->sValue[0]!='\'' &&!TrieSearchIn(lexer->keyWords,lexer->tokenizer->sValue);
}
void LexerEatDelim(Lexer*lexer,char c){
    StreamTokenizerNext(lexer->tokenizer);
}
int LexerEatIntConstant(Lexer *lexer){
    int i = lexer->tokenizer->iValue;
    StreamTokenizerNext(lexer->tokenizer);
    return i;
}
char* LexerEatStringConstant(Lexer *lexer){
    char * i = strdup(lexer->tokenizer->sValue);
    StreamTokenizerNext(lexer->tokenizer);
    return i;
}
void LexerEatKeyWord(Lexer*lexer,char* s){
    StreamTokenizerNext(lexer->tokenizer);
}
char* LexerEatIDConstant(Lexer *lexer){
    char * i = strdup(lexer->tokenizer->sValue);
    StreamTokenizerNext(lexer->tokenizer);
    return i;
}
char * LexerEatId(Lexer*lexer){
    char * s = strdup(lexer->tokenizer->sValue);
    StreamTokenizerNext(lexer->tokenizer);
    return strdup(s);
}