//
// Created by yaohuayuan on 2024/11/20.
//
// StreamTokenizer.c

#include "StreamTokenizer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "StreamTokenizer.h"

StreamTokenizer* StreamTokenizerInit(const char *input) {
    if (!input) return NULL;
    StreamTokenizer *st = (StreamTokenizer*)malloc(sizeof(StreamTokenizer));
    st->input = input;
    st->pos = input;
    st->start = NULL;
    st->length = 0;
    st->intValue = 0;
    st->type = STT_EOF;
    return st;
}

void StreamTokenizerFree(StreamTokenizer *st) {
    if (st) free(st);
}

// 跳过空格和 SQL 注释
static void skipWhitespaceAndComments(StreamTokenizer *st) {
    while (*st->pos) {
        if (isspace((unsigned char)*st->pos)) {
            st->pos++;
        } else if (*st->pos == '-' && *(st->pos + 1) == '-') {
            st->pos += 2;
            while (*st->pos && *st->pos != '\n') st->pos++;
        } else if (*st->pos == '/' && *(st->pos + 1) == '*') {
            // 额外支持多行注释 /* ... */
            st->pos += 2;
            while (*st->pos && !(*st->pos == '*' && *(st->pos + 1) == '/')) st->pos++;
            if (*st->pos) st->pos += 2;
        } else {
            break;
        }
    }
}

int StreamTokenizerNext(StreamTokenizer *st) {
    skipWhitespaceAndComments(st);
    if (*st->pos == '\0') return (st->type = STT_EOF);

    st->start = st->pos;
    char c = *st->pos;

    // 1. 识别 WORD (标识符)
    if (isalpha((unsigned char)c) || c == '_') {
        while (isalnum((unsigned char)*st->pos) || *st->pos == '_') st->pos++;
        st->length = (int)(st->pos - st->start);
        return (st->type = STT_WORD);
    }

    // 2. 识别 NUMBER (长整数)
    if (isdigit((unsigned char)c)) {
        st->intValue = 0;
        while (isdigit((unsigned char)*st->pos)) {
            st->intValue = st->intValue * 10 + (*st->pos - '0');
            st->pos++;
        }
        st->length = (int)(st->pos - st->start);
        return (st->type = STT_NUMBER);
    }

    // 3. 识别 STRING (处理未闭合错误和转义)
    if (c == '\'') {
        st->pos++; // 跳过开头的 '
        st->start = st->pos;
        while (*st->pos) {
            if (*st->pos == '\'' && *(st->pos + 1) == '\'') {
                st->pos += 2; // SQL 转义 ''
            } else if (*st->pos == '\'') {
                break;
            } else {
                st->pos++;
            }
        }
        if (*st->pos == '\'') {
            st->length = (int)(st->pos - st->start);
            st->pos++; // 跳过结尾的 '
            return (st->type = STT_STRING);
        }
        return (st->type = STT_ERROR); // 引号未闭合
    }

    // 4. 识别多字符运算符 (DELIM)
    st->pos++;
    // 检查 >=, <=, !=, <>
    if ((c == '>' || c == '<' || c == '!') && *st->pos == '=') {
        st->pos++;
    } else if (c == '<' && *st->pos == '>') {
        st->pos++;
    }

    st->length = (int)(st->pos - st->start);
    return (st->type = STT_DELIM);
}