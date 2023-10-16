
#pragma once

#include "cbased/core/base.h"

typedef struct string string;

struct string {
  char *data;
  u64   size;
};

INLINE string
str_lit(const char *c)
{
  string result;
  result.data = (char *)c;
  result.size = strlen(c);
  return result;
}

INLINE b32
str_eq(const string left, const string right)
{
  b32 result = !strcmp(left.data, right.data);
  return result;
}
