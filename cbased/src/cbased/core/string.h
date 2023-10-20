
#pragma once

#include "cbased/core/base.h"

typedef struct string string;

struct string {
  char *str;
  u64   size;
};

inline string
str_lit(const char *c)
{
  string result;
  result.str  = (char *)c;
  result.size = strlen(c);
  return result;
}

inline b32
str_eq(const string left, const string right)
{
  b32 result = !strcmp(left.str, right.str);
  return result;
}
