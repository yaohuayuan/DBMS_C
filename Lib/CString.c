//
// Created by yaohuayuan on 2025/5/30.
//

#include "CString.h"
CString* c_string_init(char *data) {
    int length = strlen(data);
    int capacity = 2;
    while (capacity < length + 1) {
        capacity *= 2;
    }

    CString *c_string = malloc(sizeof(CString));
    c_string->data = malloc(capacity);
    strcpy(c_string->data, data);
    c_string->length = length;
    c_string->capacity = capacity;
    return c_string;
}

void c_string_free(CString *c_string) {
    free(c_string->data);
    free(c_string);
}
CString* c_string_copy(CString *data){
    CString *c_string = malloc(sizeof(CString));
    c_string->data = malloc(data->capacity);
    memcpy(c_string->data, data->data, data->length);
    c_string->length = data->length;
    c_string->capacity = data->capacity;
    return c_string;
}
int c_string_compare(CString *c_string1, CString *c_string2) {
    for (int i = 0; i < MIN(c_string1->length, c_string2->length); i++) {
        if (c_string1->data[i] != c_string2->data[i]) {
            return c_string1->data[i] - c_string2->data[i];
        }
    }
    return c_string1->length - c_string2->length;
}

int c_string_insert(CString *c_string1, CString *c_string2, int index) {
    if (index > c_string1->length || index < 0) {
        return 0;
    }
    int new_length = c_string1->length + c_string2->length;
    while (c_string1->capacity < new_length + 1) {
        c_string1->capacity *= 2;
        c_string1->data = realloc(c_string1->data, c_string1->capacity);
    }
    memmove(
            c_string1->data + index + c_string2->length,
            c_string1->data + index,
            c_string1->length - index
    );
    memcpy(c_string1->data + index, c_string2->data, c_string2->length);
    c_string1->length = new_length;
    c_string1->data[new_length] = '\0';
    return 1;
}

int c_string_delete(CString *c_string, int index) {
    if (index < 0 || index >= c_string->length) {
        return 0;
    }
    memmove(
            c_string->data + index,
            c_string->data + index + 1,
            c_string->length - index
    );
    c_string->length -= 1;
    c_string->data[c_string->length] = '\0';
    return 1;
}

int c_string_append(CString *c_string1, CString *c_string2) {
    return c_string_insert(c_string1, c_string2, c_string1->length);
}