//
// Created by yaohuayuan on 2024/11/28.
//

#include "Parser.h"
#include "Expression.h"
#include "Term.h"
Parser *ParserInit(char *s){
    Parser  *parser = malloc(sizeof (Parser));
    parser->lexer = LexerInit(s);

//    printf("test");
    StreamTokenizerNext(parser->lexer->tokenizer);
//    printf("test");
    return parser;
}
char *ParserField(Parser*parser){
    return LexerEatId(parser->lexer);
}
Constant *ParserConstant(Parser*parser){
    if(LexerMatchStringConstant(parser->lexer)){
        return ConstantInitString(LexerEatStringConstant(parser->lexer));
    }else{
        return ConstantInitInt(LexerEatIntConstant(parser->lexer));
    }
}
Expression *ParserExpression(Parser*parser){
    if(LexerMatchId(parser->lexer)){
        return ExpressionInitFieldName(ParserField(parser));
    }else{
        return ExpressionInitConstant(ParserConstant(parser));
    }
}
Term *ParserTerm(Parser*parser){
    Expression *lhs = ParserExpression(parser);
    LexerEatDelim(parser->lexer,'=');
    Expression *rhs = ParserExpression(parser);
    return TermInit(lhs,rhs);
}
Predicate *ParserPredicate(Parser*parser){
    Predicate * predicate = PredicateInit(ParserTerm(parser));
    if(LexerMatchKeyWord(parser->lexer,"and")){
        LexerEatKeyWord(parser->lexer,"and");
        PredicateConjoinWith(predicate, ParserPredicate(parser));
    }
    return predicate;
}
List* ParserSelectList(Parser*parser){
    List* l = ListInit(LIST_TYPE_STRING);
    ListAppend(l, ParserField(parser),sizeof(ParserField(parser)));
    if(LexerMatchDelim(parser->lexer,',')){
        LexerEatDelim(parser->lexer,',');
        ListAddAll(l, ParserSelectList(parser));
    }
    return l;
}
List* ParserTabletList(Parser*parser){
    List* l = ListInit(LIST_TYPE_STRING);
    ListAppend(l,LexerEatId(parser->lexer),sizeof(LexerEatId(parser->lexer)));
    if(LexerMatchDelim(parser->lexer,',')){
        LexerEatDelim(parser->lexer,',');
        ListAddAll(l, ParserSelectList(parser));
    }
    return l;
}
QueryData *ParserQuery(Parser*parser){
    LexerEatKeyWord(parser->lexer,"select");
    List *fields = ParserSelectList(parser);
    LexerEatKeyWord(parser->lexer,"from");
    List*tables = ParserTabletList(parser);
    Predicate *predicate = PredicateInit(NULL);
    if(LexerMatchKeyWord(parser->lexer,"where")){
        LexerEatKeyWord(parser->lexer,"where");
        predicate = ParserPredicate(parser);
    }
    return QueryDataInit(fields,tables,predicate);
}
CommandData* ParserDelete(Parser*parser){
    LexerEatKeyWord(parser->lexer,"delete");
    LexerEatKeyWord(parser->lexer,"from");
    char *tblname = LexerEatId(parser->lexer);
    Predicate *predicate = PredicateInit(NULL);
    if(LexerMatchKeyWord(parser->lexer,"where")){
        LexerEatKeyWord(parser->lexer,"where");
        predicate = ParserPredicate(parser);
    }
    CommandData * commandData = malloc(sizeof(CommandData));
    commandData->code = CMD_DELETE_DATA;
    commandData->data.deleteData = DeleteDataInit(tblname,predicate);
    return commandData;
}
List* ParserFieldList(Parser *parser){
    List *l = ListInit(LIST_TYPE_STRING);
    ListAppend(l, ParserField(parser),sizeof (ParserField(parser)));
    if(LexerMatchDelim(parser->lexer,',')){
        LexerEatDelim(parser->lexer,',');
        ListAddAll(l, ParserFieldList(parser));
    }
    return l;
}
List* ParserConstantList(Parser *parser){
    List *l = ListInit(LIST_TYPE_CONSTANT);
    ListAppend(l, ParserConstant(parser),sizeof(Constant));
    if(LexerMatchDelim(parser->lexer,',')){
        LexerEatDelim(parser->lexer,',');
        ListAddAll(l, ParserConstantList(parser));
    }
    return l;
}
CommandData* ParserInsert(Parser*parser){
    LexerEatKeyWord(parser->lexer,"insert");
    LexerEatKeyWord(parser->lexer,"into");
    char *tblname = LexerEatId(parser->lexer);
    LexerEatDelim(parser->lexer,'(');
    List *fld = ParserFieldList(parser);
    LexerEatDelim(parser->lexer,')');
    LexerEatKeyWord(parser->lexer,"values");
    LexerEatDelim(parser->lexer,'(');
    List *vals = ParserConstantList(parser);
    CommandData * commandData = malloc(sizeof(CommandData));
    commandData->code = CMD_INSERT_DATA;
    commandData->data.insertData = InsertDataInit(tblname,fld,vals);
    return commandData;
}
CommandData* ParserModify(Parser*parser){
    LexerEatKeyWord(parser->lexer,"update");
    char *tblname = LexerEatId(parser->lexer);
    LexerEatKeyWord(parser->lexer,"set");
    char *fldname = ParserField(parser);
    LexerEatDelim(parser->lexer,'=');
    Expression *expression = ParserExpression(parser);
    Predicate *predicate = PredicateInit(NULL);
    if(LexerMatchKeyWord(parser->lexer,"where")){
        LexerEatKeyWord(parser->lexer,"where");
        predicate = ParserPredicate(parser);
    }

    CommandData * commandData = malloc(sizeof(CommandData));
    commandData->code = CMD_MODIFY_DATA;
    commandData->data.modifyData = ModifyDataInit(tblname,fldname,expression,predicate);
    return commandData;
}
Schema *ParserFieldType(Parser *parser,char *fldname){
    Schema *schema = SchemaInit();
    if(LexerMatchKeyWord(parser->lexer,"int")){
        LexerEatKeyWord(parser->lexer,"int");
        SchemaAddIntField(schema,fldname);
    }else{
        LexerEatKeyWord(parser->lexer,"varchar");
        LexerEatDelim(parser->lexer,'(');
        int strlen = LexerEatIntConstant(parser->lexer);
        LexerEatDelim(parser->lexer,')');
        SchemaAddStringField(schema,fldname,strlen);
    }
    return schema;

}
Schema *ParserFieldDef(Parser*parser){
    char *fldname = ParserField(parser);
    return ParserFieldType(parser,fldname);
}
Schema *ParserFieldDefs(Parser*parser){
    Schema*schema = ParserFieldDef(parser);
    if(LexerMatchDelim(parser->lexer,',')){
        LexerEatDelim(parser->lexer,',');
        Schema  *schema1 = ParserFieldDefs(parser);
        SchemaAddAll(schema,schema1);
    }
    return schema;
}
CommandData* ParserCreateTable(Parser*parser){
    LexerEatKeyWord(parser->lexer,"table");
    char *tblname = LexerEatId(parser->lexer);
    LexerEatDelim(parser->lexer,'(');
    Schema *schema = ParserFieldDefs(parser);
    LexerEatDelim(parser->lexer,')');

    CommandData * commandData = malloc(sizeof(CommandData));
    commandData->code = CMD_CREATE_TABLE;
    commandData->data.createTableData = CreateTableDataInit(tblname,schema);
    return commandData;
}
CommandData* ParserCreateView(Parser*parser){
    LexerEatKeyWord(parser->lexer,"view");
    char *tblname = LexerEatId(parser->lexer);
    LexerEatKeyWord(parser->lexer,"as");
    QueryData *queryData = ParserQuery(parser);


    CommandData * commandData = malloc(sizeof(CommandData));
    commandData->code = CMD_CREATE_VIEW;
    commandData->data.createViewData = CreateViewDataInit(tblname,queryData);
    return commandData;
}
CommandData* ParserCreateIndex(Parser*parser){
    LexerEatKeyWord(parser->lexer,"index");
    char *idxname = LexerEatId(parser->lexer);
    LexerEatKeyWord(parser->lexer,"on");
    char *tblname = LexerEatId(parser->lexer);
    LexerEatDelim(parser->lexer,'(');
    char *fldname = ParserField(parser);
    LexerEatDelim(parser->lexer,')');


    CommandData * commandData = malloc(sizeof(CommandData));
    commandData->code = CMD_CREATE_INDEX;
    commandData->data.createIndexData = CreateIndexDataInit(idxname,tblname,fldname);
    return commandData;
}
CommandData *ParserCreate(Parser*parser){
    LexerEatKeyWord(parser->lexer,"create");
    if(LexerMatchKeyWord(parser->lexer,"table")){
        return ParserCreateTable(parser);
    }else if(LexerMatchKeyWord(parser->lexer,"view")){
        return ParserCreateView(parser);
    }else{
        return ParserCreateIndex(parser);
    }
}
CommandData *ParserUpdateCmd(Parser*parser){
    if(LexerMatchKeyWord(parser->lexer,"insert")){
        return ParserInsert(parser);
    }else if(LexerMatchKeyWord(parser->lexer,"delete")){
        return ParserDelete(parser);
    }else if(LexerMatchKeyWord(parser->lexer,"update")){
        return ParserModify(parser);
    }else{
        return ParserCreate(parser);
    }
}