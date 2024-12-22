//
// Created by yaohuayuan on 2024/11/28.
//

#ifndef DBMS_C_PARSER_H
#define DBMS_C_PARSER_H
#include "Lexer.h"
#include "Constant.h"
#include "Predicate.h"
#include "QueryData.h"
#include "InsertData.h"
#include "DeleteData.h"
#include "ModifyData.h"
#include "CreateIndexData.h"
#include "CreateViewData.h"
#include "CreateTableData.h"
typedef struct Parser{
    Lexer *lexer;
}Parser;
typedef enum CommandCode {
    CMD_INSERT_DATA,
    CMD_DELETE_DATA,
    CMD_MODIFY_DATA,
    CMD_CREATE_INDEX,
    CMD_CREATE_VIEW,
    CMD_CREATE_TABLE,
} CommandCode;

typedef union CommandUnion {
    InsertData *insertData;
    DeleteData *deleteData;
    ModifyData *modifyData;
    CreateIndexData *createIndexData;
    CreateViewData *createViewData;
    CreateTableData *createTableData;
} CommandUnion;

typedef struct CommandData {
    CommandUnion data;  // 使用共用体存储不同命令的数据
    CommandCode code;   // 表示具体命令类型
} CommandData;

/**
 * 初始化解析器
 * @param s 待解析的字符串
 * @return 返回解析器指针
 */
Parser *ParserInit(char *s);

/**
 * 解析字段名
 * @param parser 解析器指针
 * @return 返回解析出的字段名字符串
 */
char *ParserField(Parser*parser);

/**
 * 解析常量值
 * @param parser 解析器指针
 * @return 返回解析出的常量值结构体指针
 */
Constant *ParserConstant(Parser*parser);

/**
 * 解析表达式
 * @param parser 解析器指针
 * @return 返回解析出的表达式结构体指针
 */
Expression *ParserExpression(Parser*parser);

/**
 * 解析项
 * @param parser 解析器指针
 * @return 返回解析出的项结构体指针
 */
Term *ParserTerm(Parser*parser);

/**
 * 解析谓词
 * @param parser 解析器指针
 * @return 返回解析出的谓词结构体指针
 */
Predicate *ParserPredicate(Parser*parser);

/**
 * 解析选择列表
 * @param parser 解析器指针
 * @return 返回解析出的选择列表结构体指针
 */
List* ParserSelectList(Parser*parser);

/**
 * 解析表列表
 * @param parser 解析器指针
 * @return 返回解析出的表列表结构体指针
 */
List* ParserTabletList(Parser*parser);

/**
 * 解析查询语句
 * @param parser 解析器指针
 * @return 返回解析出的查询数据结构体指针
 */
QueryData *ParserQuery(Parser*parser);

/**
 * 解析删除语句
 * @param parser 解析器指针
 * @return 返回解析出的命令数据结构体指针
 */
CommandData* ParserDelete(Parser*parser);

/**
 * 解析字段列表
 * @param parser 解析器指针
 * @return 返回解析出的字段列表结构体指针
 */
List* ParserFieldList(Parser *parser);

/**
 * 解析常量列表
 * @param parser 解析器指针
 * @return 返回解析出的常量列表结构体指针
 */
List* ParserConstantList(Parser *parser);

/**
 * 解析插入语句
 * @param parser 解析器指针
 * @return 返回解析出的命令数据结构体指针
 */
CommandData* ParserInsert(Parser*parser);

/**
 * 解析修改语句
 * @param parser 解析器指针
 * @return 返回解析出的命令数据结构体指针
 */
CommandData* ParserModify(Parser*parser);

/**
 * 根据字段名解析字段类型
 * @param parser 解析器指针
 * @param fldname 字段名字符串
 * @return 返回解析出的字段类型结构体指针
 */
Schema *ParserFieldType(Parser *parser,char *fldname);

/**
 * 解析字段定义
 * @param parser 解析器指针
 * @return 返回解析出的字段定义结构体指针
 */
Schema *ParserFieldDef(Parser*parser);

/**
 * 解析字段定义列表
 * @param parser 解析器指针
 * @return 返回解析出的字段定义列表结构体指针
 */
Schema *ParserFieldDefs(Parser*parser);

/**
 * 解析创建表语句
 * @param parser 解析器指针
 * @return 返回解析出的命令数据结构体指针
 */
CommandData* ParserCreateTable(Parser*parser);

/**
 * 解析创建视图语句
 * @param parser 解析器指针
 * @return 返回解析出的命令数据结构体指针
 */
CommandData* ParserCreateView(Parser*parser);

/**
 * 解析创建索引语句
 * @param parser 解析器指针
 * @return 返回解析出的命令数据结构体指针
 */
CommandData* ParserCreateIndex(Parser*parser);

/**
 * 解析创建语句（通用）
 * @param parser 解析器指针
 * @return 返回解析出的命令数据结构体指针
 */
CommandData *ParserCreate(Parser*parser);

/**
 * 解析更新命令
 * @param parser 解析器指针
 * @return 返回解析出的命令数据结构体指针
 */
CommandData *ParserUpdateCmd(Parser*parser);

#endif //DBMS_C_PARSER_H
