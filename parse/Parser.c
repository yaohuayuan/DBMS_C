//
// Created by yaohuayuan on 2024/11/28.
//

#include "Parser.h"
#include "Expression.h"
#include "Term.h"

static bool CStringListEquals(const void *a, const void *b) {
    return CStringEqual((const CString *)a, (const CString *)b) != 0;
}
Parser *ParserInit(const char *s){
    Parser  *parser = malloc(sizeof (Parser));
    parser->lexer = LexerInit(s);
    return parser;
}
CString *ParserField(Parser*parser){
    return CStringCreateFromCStr(LexerEatId(parser->lexer));
}
Constant *ParserConstant(Parser*parser){
    if(LexerMatchStringConstant(parser->lexer)){
        return ConstantInitString(LexerEatStringConstant(parser->lexer));
    }else{
        return ConstantInitInt(LexerEatIntConstant(parser->lexer));
    }
}
Expression *ParserExpression(Parser*parser){
    if (LexerMatchId(parser->lexer)) {

        char *id1 = LexerEatId(parser->lexer);

        // 🔥 关键：判断是不是 table.field
        if (LexerMatchDelim(parser->lexer, '.')) {

            LexerEatDelim(parser->lexer, '.');

            if (!LexerMatchId(parser->lexer)) {
                fprintf(stderr, "Syntax Error: expected field after '.'\n");
                exit(1);
            }

            char *id2 = LexerEatId(parser->lexer);

            Expression *expr = ExpressionInitFieldRef(id1, id2);

            free(id1);
            free(id2);

            return expr;
        }
        Expression *expr = ExpressionInitFieldName(id1);
        free(id1);
        return expr;

    } else {
        return ExpressionInitConstant(ParserConstant(parser));
    }
}
Term *ParserTerm(Parser*parser){
    Expression *lhs = ParserExpression(parser);
    const CompareOp op = LexerGetCurrentOp(parser->lexer);
    LexerNextToken(parser->lexer);
    Expression *rhs = ParserExpression(parser);
    return TermInit(lhs,rhs,op);
}
Predicate *ParserPredicate(Parser*parser){
    Predicate * predicate = PredicateInit(ParserTerm(parser));
    if(LexerMatchKeyWord(parser->lexer,"and")){
        LexerEatKeyWord(parser->lexer,"and");
        PredicateConjoinWith(predicate, ParserPredicate(parser));
    }
    return predicate;
}


CList* ParserSelectList(Parser*parser){
    CList* l = CListInit(NULL, CStringListEquals, NULL);

    if (LexerMatchDelim(parser->lexer, '*')) {
        LexerEatDelim(parser->lexer, '*');
        CListAppend(l, CStringCreateFromCStr("*"));
        return l;
    }

    CListAppend(l, ParserField(parser));

    if(LexerMatchDelim(parser->lexer,',')){
        LexerEatDelim(parser->lexer,',');
        CListAddAll(l, ParserSelectList(parser));
    }
    return l;
}
CList* ParserTabletList(Parser*parser){
    CList* l = CListInit(NULL, CStringListEquals, NULL);
    CString *tblname = CStringCreateFromCStr(LexerEatId(parser->lexer));
    CListAppend(l, tblname);
    if(LexerMatchDelim(parser->lexer,',')){
        LexerEatDelim(parser->lexer,',');
        CListAddAll(l, ParserTabletList(parser));
    }
    return l;
}
QueryData *ParserQuery(Parser*parser){
    LexerEatKeyWord(parser->lexer,"select");
    CList *fields = ParserSelectList(parser);
    LexerEatKeyWord(parser->lexer,"from");
    CList*tables = ParserTabletList(parser);
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
    CString *tblname = CStringCreateFromCStr(LexerEatId(parser->lexer));
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
CList* ParserFieldList(Parser *parser){
    CList *l = CListInit(NULL, CStringListEquals, NULL);
    CListAppend(l, ParserField(parser));
    if(LexerMatchDelim(parser->lexer,',')){
        LexerEatDelim(parser->lexer,',');
        CListAddAll(l, ParserFieldList(parser));
    }
    return l;
}
CList* ParserConstantList(Parser *parser){
    CList *l = CListInit(NULL, NULL, NULL);
    CListAppend(l, ParserConstant(parser));
    if(LexerMatchDelim(parser->lexer,',')){
        LexerEatDelim(parser->lexer,',');
        CListAddAll(l, ParserConstantList(parser));
    }
    return l;
}
CommandData* ParserInsert(Parser*parser){
    LexerEatKeyWord(parser->lexer,"insert");
    LexerEatKeyWord(parser->lexer,"into");
    CString *tblname = CStringCreateFromCStr(LexerEatId(parser->lexer));
    LexerEatDelim(parser->lexer,'(');
    CList *fld = ParserFieldList(parser);
    LexerEatDelim(parser->lexer,')');
    LexerEatKeyWord(parser->lexer,"values");
    LexerEatDelim(parser->lexer,'(');
    CList *vals = ParserConstantList(parser);
    LexerEatDelim(parser->lexer,')');
    CommandData * commandData = malloc(sizeof(CommandData));
    commandData->code = CMD_INSERT_DATA;
    commandData->data.insertData = InsertDataInit(tblname,fld,vals);

    return commandData;
}
CommandData* ParserModify(Parser*parser){
    LexerEatKeyWord(parser->lexer,"update");
    CString *tblname = CStringCreateFromCStr(LexerEatId(parser->lexer));
    LexerEatKeyWord(parser->lexer,"set");
    CString *fldname = ParserField(parser);
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
Schema *ParserFieldType(Parser *parser,CString *fldname){
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
    CString *fldname = ParserField(parser);
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
    CString *tblname = CStringCreateFromCStr(LexerEatId(parser->lexer));
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
    CString *tblname = CStringCreateFromCStr(LexerEatId(parser->lexer));
    LexerEatKeyWord(parser->lexer,"as");
    QueryData *queryData = ParserQuery(parser);


    CommandData * commandData = malloc(sizeof(CommandData));
    commandData->code = CMD_CREATE_VIEW;
    commandData->data.createViewData = CreateViewDataInit(tblname,queryData);
    return commandData;
}
CommandData* ParserCreateIndex(Parser*parser){
    LexerEatKeyWord(parser->lexer,"index");
    CString *idxname = CStringCreateFromCStr(LexerEatId(parser->lexer));
    LexerEatKeyWord(parser->lexer,"on");
    CString *tblname = CStringCreateFromCStr(LexerEatId(parser->lexer));
    LexerEatDelim(parser->lexer,'(');
    CString *fldname = ParserField(parser);
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