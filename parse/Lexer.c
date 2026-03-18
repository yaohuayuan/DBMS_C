#include "Lexer.h"
#include <strings.h>


static char* my_strndup(const char* s, size_t n) {
    char* new_str = (char*)malloc(n + 1);
    if (new_str) {
        memcpy(new_str, s, n);
        new_str[n] = '\0';
    }
    return new_str;
}

Lexer* LexerInit(const char *s) {
    Lexer *lexer = malloc(sizeof(Lexer));
    lexer->tokenizer = StreamTokenizerInit(s);

    // 初始化预读
    StreamTokenizerNext(lexer->tokenizer);

    // 检查初始化时是否有错误 (如第一个字符就是非法字符)
    if (lexer->tokenizer->type == STT_ERROR) {
        fprintf(stderr, "Lexical Error: Invalid character at start.\n");
    }
    return lexer;
}

void LexerFree(Lexer *lexer) {
    StreamTokenizerFree(lexer->tokenizer);
    free(lexer);
}

// 2. 优化的关键字匹配 (减少 strlen 开销)
bool LexerMatchKeyWord(Lexer *l, const char *kw) {
    StreamTokenizer *st = l->tokenizer;
    if (st->type != STT_WORD) return false;

    size_t kw_len = strlen(kw);
    if (st->length != (int)kw_len) return false;

    return strncasecmp(st->start, kw, kw_len) == 0;
}

bool LexerMatchId(Lexer *l) {
    // 标识符定义：是单词，且后续在 Parser 逻辑中不匹配任何关键字
    return l->tokenizer->type == STT_WORD;
}

bool LexerMatchDelim(Lexer *l, char d) {
    // 注意：st->start[0] 匹配单字符符号，length == 1 排除多字符运算符
    return l->tokenizer->type == STT_DELIM &&
           l->tokenizer->length == 1 &&
           l->tokenizer->start[0] == d;
}

// 3. 健壮的 Eat 系列
char* LexerEatId(Lexer *l) {
    if (!LexerMatchId(l)) {
        fprintf(stderr, "Syntax Error: Expected Identifier near '%.*s'\n",
                l->tokenizer->length, l->tokenizer->start);
        exit(1);
    }
    char *s = my_strndup(l->tokenizer->start, l->tokenizer->length);
    StreamTokenizerNext(l->tokenizer);
    return s;
}

char* LexerEatStringConstant(Lexer *l) {
    if (l->tokenizer->type != STT_STRING) {
        fprintf(stderr, "Syntax Error: Expected String constant\n");
        exit(1);
    }
    char *s = my_strndup(l->tokenizer->start, l->tokenizer->length);
    StreamTokenizerNext(l->tokenizer);
    return s;
}

void LexerEatKeyWord(Lexer *l, const char *kw) {
    if (!LexerMatchKeyWord(l, kw)) {
        fprintf(stderr, "Syntax Error: Expected keyword '%s', but found '%.*s'\n",
                kw, l->tokenizer->length, l->tokenizer->start);
        exit(1);
    }
    StreamTokenizerNext(l->tokenizer);
}
// Lexer.c
CompareOp LexerGetCurrentOp(Lexer *l) {
    StreamTokenizer *st = l->tokenizer;
    if (st->type != STT_DELIM) return OP_UNKNOWN;

    // 所有的复杂逻辑（看长度、看字符）全部锁在 Lexer 内部
    if (st->length == 1) {
        char c = st->start[0];
        if (c == '=') return OP_EQ;
        if (c == '>') return OP_GT;
        if (c == '<') return OP_LT;
    } else if (st->length == 2) {
        if (strncmp(st->start, ">=", 2) == 0) return OP_GE;
        if (strncmp(st->start, "<=", 2) == 0) return OP_LE;
        if (strncmp(st->start, "!=", 2) == 0 || strncmp(st->start, "<>", 2) == 0) return OP_NE;
    }
    return OP_UNKNOWN;
}
void LexerNextToken(Lexer *l) {
    StreamTokenizerNext(l->tokenizer);
}
// 补全：判断是否为整数常量
bool LexerMatchIntConstant(Lexer *l) {
    return l->tokenizer->type == STT_NUMBER;
}

// 补全：判断是否为字符串常量（你之前代码可能漏了这一个逻辑）
bool LexerMatchStringConstant(Lexer *l) {
    return l->tokenizer->type == STT_STRING;
}

// 补全：消费整数并返回其值
int LexerEatIntConstant(Lexer *l) {
    if (!LexerMatchIntConstant(l)) {
        fprintf(stderr, "Syntax Error: Expected Integer constant\n");
        exit(1);
    }
    int val = (int)l->tokenizer->intValue;
    StreamTokenizerNext(l->tokenizer); // 别忘了翻页
    return val;
}

// 补全：消费指定的分隔符
void LexerEatDelim(Lexer *l, char d) {
    if (!LexerMatchDelim(l, d)) {
        fprintf(stderr, "Syntax Error: Expected delimiter '%c', but found '%.*s'\n",
                d, l->tokenizer->length, l->tokenizer->start);
        exit(1);
    }
    StreamTokenizerNext(l->tokenizer); // 消费掉它
}