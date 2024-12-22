//
// Created by yaohuayuan on 2024/11/30.
//

#include "PredParser.h"
PredParser *PredParserInit(char *s){
    PredParser *predParser = malloc(sizeof (PredParser));
    predParser->lexer = LexerInit(s);
    return predParser;
}
char *PredParserField(PredParser*predParser){
    return LexerEatId(predParser->lexer);
}
void PredParseConstant(PredParser*predParser){
    if(LexerMatchStringConstant(predParser->lexer)){
        LexerEatStringConstant(predParser->lexer);
    }else
        LexerEatIntConstant(predParser->lexer);
}
void PredParseExpression(PredParser*predParser){
    if(LexerMatchId(predParser->lexer)){
        PredParserField(predParser);
    }else
        PredParseConstant(predParser);
}
void PredParseTerm(PredParser*predParser){
    PredParseExpression(predParser);
    LexerEatDelim(predParser->lexer,'=');
    PredParseExpression(predParser);
}
void PredParsePredicate(PredParser*predParser){
    PredParseTerm(predParser);
    if(LexerMatchKeyWord(predParser->lexer,"and")){
        LexerEatKeyWord(predParser->lexer,"and");
        PredParsePredicate(predParser);
    }
}