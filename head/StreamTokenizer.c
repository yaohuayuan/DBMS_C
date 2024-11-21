//
// Created by yaohuayuan on 2024/11/20.
//
// StreamTokenizer.c

#include "StreamTokenizer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


char *strndup(const char *s, size_t len) {
    size_t n = strnlen(s, len);  // 获取字符串的长度，最多到 len 个字符。
    char *dup = malloc(n + 1);    // 为复制的字符串分配内存。
    if (dup) {
        memcpy(dup, s, n);       // 复制字符串的前 n 个字符。
        dup[n] = '\0';            // 手动添加字符串的结束符。
    }
    return dup;
}
void StreamTokenizerWork(StreamTokenizer *tokenizer,char *s){
    tokenizer->data = strdup(s);
    StreamTokenizerType(tokenizer);
}
// 初始化 StreamTokenizer
StreamTokenizer *StreamTokenizerInit(char *s) {
    StreamTokenizer *streamTokenizer = malloc(sizeof(StreamTokenizer));
    if (!streamTokenizer) return NULL;

    streamTokenizer->data = strdup(s);
    streamTokenizer->keySet = malloc(sizeof(map_int_t));
    map_init(streamTokenizer->keySet);

    // 设置字母对应 TOKEN_WORD 类型
    for (int i = 0; i < 26; i++) {
        char key[2];
        key[0] = (char)('a' + i);
        key[1] = '\0';
        map_set(streamTokenizer->keySet, key, TOKEN_WORD);
        key[0] = (char)('A' + i);
        map_set(streamTokenizer->keySet, key, TOKEN_WORD);
    }
    char tem[2]={'\'','\0'};
    map_set(streamTokenizer->keySet, tem, TOKEN_WORD);

    // 设置数字对应 TOKEN_INT_CONSTANT 类型
    for (int i = 0; i < 10; i++) {
        char key[2];
        key[0] = (char)('0' + i);
        key[1] = '\0';
        map_set(streamTokenizer->keySet, key, TOKEN_INT_CONSTANT);
    }

    // 设置分隔符对应 TOKEN_DELIM 类型
    char *delimiters[] = { ",", ";", "(", ")", "{", "}", "[", "]", ".", "=", "+", "-", "*", "/", "%"};
    for (int i = 0; i < sizeof(delimiters) / sizeof(delimiters[0]); i++) {
        map_set(streamTokenizer->keySet, delimiters[i], TOKEN_DELIM);
    }
    char key[2];
    key[0]=' ';
    key[1] = '\0';
    map_set(streamTokenizer->keySet, key, TOKEN_NULL);
    // 初始化 Token ID 数据
    streamTokenizer->streamTokenizerTokenId = StreamTokenizerTokenIdInit();

    streamTokenizer->sValue = NULL;
    streamTokenizer->iValue = 0;
    streamTokenizer->type = TOKEN_UNKNOWN;
    StreamTokenizerType(streamTokenizer);
    return streamTokenizer;
}

// 转换为小写模式
void StreamTokenizerLowerCaseMode(StreamTokenizer *streamTokenizer) {
    for (int i = 0; streamTokenizer->data[i] != '\0'; i++) {
        if (streamTokenizer->data[i] >= 'A' && streamTokenizer->data[i] <= 'Z') {
            streamTokenizer->data[i] += 32;
        }
    }
}

// 添加普通字符到 keySet
void StreamTokenizerOrdinaryChar(StreamTokenizer *streamTokenizer, char ch) {
    char key[2] = {ch, '\0'};
    map_set(streamTokenizer->keySet, key, TOKEN_DELIM);
    StreamTokenizerType(streamTokenizer);
}

// 添加单词字符到 keySet
void StreamTokenizerWordChars(StreamTokenizer *streamTokenizer, char start, char end) {
    for (char ch = start; ch <= end; ch++) {
        char key[2] = {ch, '\0'};
        map_set(streamTokenizer->keySet, key, TOKEN_WORD);
    }
    StreamTokenizerType(streamTokenizer);
}
void StreamTokenizerTokenIDAdd(StreamTokenizerTokenID *tokenID, int pos, TokenType type) {
    // 检查是否需要扩展容量
    if (tokenID->len >= tokenID->maxLen) {
        tokenID->maxLen *= 2; // 容量加倍
        tokenID->data = realloc(tokenID->data, tokenID->maxLen * sizeof(StreamTokenizerTokenNode));
        if (!tokenID->data) {
            perror("Failed to reallocate memory for StreamTokenizerTokenID");
            exit(EXIT_FAILURE);
        }
    }

    // 添加新元素
    tokenID->data[tokenID->len].pos = pos;
    tokenID->data[tokenID->len].len = 0;
    tokenID->endLen++;
    tokenID->data[tokenID->len].type = type;
    tokenID->len++;
}
void StreamTokenizerTokenIDModify(StreamTokenizerTokenID *tokenID, int length,int pos, TokenType type) {
    tokenID->data[length].pos = pos;
    tokenID->data[length].type = type;
}
StreamTokenizerTokenID * StreamTokenizerTokenIdInit(){
    StreamTokenizerTokenID *streamTokenizerTokenId = malloc(sizeof (StreamTokenizerTokenID));
    streamTokenizerTokenId->len = 0;
    streamTokenizerTokenId->maxLen = 4;
    streamTokenizerTokenId->endLen = 0;
    streamTokenizerTokenId->data = malloc(4 * sizeof(StreamTokenizerTokenNode));
    return streamTokenizerTokenId;
}
void StreamTokenizerTokenIdFree(StreamTokenizerTokenID*streamTokenizerTokenId){
    free(streamTokenizerTokenId->data);
    free(streamTokenizerTokenId);
}
void StreamTokenizerType(StreamTokenizer*streamTokenizer){
    StreamTokenizerTokenIdFree(streamTokenizer->streamTokenizerTokenId);
    streamTokenizer->streamTokenizerTokenId = StreamTokenizerTokenIdInit();
    int temStart = 0;
    for(int i=0;i< strlen(streamTokenizer->data);){
        char ch = streamTokenizer->data[i];
        char key[2] = {ch, '\0'};
        int* c = map_get(streamTokenizer->keySet,key);
        if(c == NULL){
            c = malloc(sizeof (int));
            *c = TOKEN_UNKNOWN;
        }
        if(*c == TOKEN_WORD ){
            StreamTokenizerTokenIDAdd(streamTokenizer->streamTokenizerTokenId,i,TOKEN_WORD);
            while (*c==TOKEN_WORD||*c==TOKEN_INT_CONSTANT){

                streamTokenizer->streamTokenizerTokenId->data[streamTokenizer->streamTokenizerTokenId->len-1].len++;
                ch = streamTokenizer->data[++i];
                char key1[2] = {ch, '\0'};
                if(ch=='\0')
                    break;
                c = map_get(streamTokenizer->keySet,key1);
                if(c == NULL){
                    c = malloc(sizeof (int));
                    *c = TOKEN_UNKNOWN;
                }
            }
        }else if(*c == TOKEN_INT_CONSTANT){
            temStart = i;
            StreamTokenizerTokenIDAdd(streamTokenizer->streamTokenizerTokenId,i,TOKEN_INT_CONSTANT);
            while (*c==TOKEN_WORD||*c==TOKEN_INT_CONSTANT){

                streamTokenizer->streamTokenizerTokenId->data[streamTokenizer->streamTokenizerTokenId->len-1].len++;
                ch = streamTokenizer->data[++i];
                char key1[2] = {ch, '\0'};
                if(ch=='\0')
                    break;
                c = map_get(streamTokenizer->keySet,key1);
                if(c == NULL){
                    c = malloc(sizeof (int));
                    *c = TOKEN_UNKNOWN;
                }
                else if(*c == TOKEN_WORD){
                    StreamTokenizerTokenIDModify(streamTokenizer->streamTokenizerTokenId,streamTokenizer->streamTokenizerTokenId->len-1,temStart,TOKEN_WORD);
                }
            }
        }else if(*c == TOKEN_DELIM){
            StreamTokenizerTokenIDAdd(streamTokenizer->streamTokenizerTokenId,i,TOKEN_DELIM);
            i++;
        }else if(*c == TOKEN_UNKNOWN){
            StreamTokenizerTokenIDAdd(streamTokenizer->streamTokenizerTokenId,i,TOKEN_UNKNOWN);
            streamTokenizer->streamTokenizerTokenId->data[streamTokenizer->streamTokenizerTokenId->len-1].len++;
            i++;
        }else if(*c == TOKEN_NULL){
            i++;
            streamTokenizer->streamTokenizerTokenId->data[streamTokenizer->streamTokenizerTokenId->len-1].len++;
        }
    }

    StreamTokenizerTokenIDAdd(streamTokenizer->streamTokenizerTokenId,strlen(streamTokenizer->data),TOKEN_END_OF);
}
void StreamTokenizerSearchReady(StreamTokenizer *streamTokenizer){
    streamTokenizer->streamTokenizerTokenId->len = 0;
}
void StreamTokenizerNext(StreamTokenizer *streamTokenizer){
    int startPos = streamTokenizer->streamTokenizerTokenId->data[streamTokenizer->streamTokenizerTokenId->len].pos;
    TokenType startType = streamTokenizer->streamTokenizerTokenId->data[streamTokenizer->streamTokenizerTokenId->len].type;
    if(startType == TOKEN_WORD||startType == TOKEN_DELIM||startType == TOKEN_UNKNOWN){
        int len = streamTokenizer->streamTokenizerTokenId->data[streamTokenizer->streamTokenizerTokenId->len].len;
        if (streamTokenizer->sValue != NULL) {
            free(streamTokenizer->sValue); // 防止内存泄漏
        }
        streamTokenizer->sValue = malloc(len + 1); // 为字符串分配内存
        memcpy(streamTokenizer->sValue, streamTokenizer->data + startPos, len);
        streamTokenizer->sValue[len] = '\0'; // 添加终止符
        streamTokenizer->type = startType;
    }else if(startType == TOKEN_INT_CONSTANT){
        int len = streamTokenizer->streamTokenizerTokenId->data[streamTokenizer->streamTokenizerTokenId->len].len;
        char *str1 = malloc(len + 1); // 分配内存，额外加1用于存储终止符
        memcpy(str1, streamTokenizer->data + startPos, len);
        str1[len] = '\0'; // 添加终止符
        streamTokenizer->iValue = atof(str1);
        free(str1); // 释放内存
        streamTokenizer->type = TOKEN_INT_CONSTANT;
    }else if(startType == TOKEN_END_OF){
        streamTokenizer->type = TOKEN_END_OF;
    }
    streamTokenizer->streamTokenizerTokenId->len++;
}