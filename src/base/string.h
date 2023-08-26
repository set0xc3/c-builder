
#pragma once

#include "defines.h"

typedef struct String
{
    char *data;
    u64   size;
} String;

typedef struct StringNode
{
    struct StringNode *next;
    String             string;
} StringNode;

typedef struct StringList
{
    StringNode *first;
    StringNode *last;
    u64         node_count;
    u64         size;
} StringList;

INLINE String
str_lit(const char *c)
{
    String result;
    result.data = (char *)c;
    result.size = strlen(c);
    return result;
}

INLINE b8
str_eq(const String left, const String right)
{
    b8 result = !strcmp(left.data, right.data);
    return result;
}
