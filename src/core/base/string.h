
#pragma once

#include "defines.h"

typedef struct String String;

struct String {
  char *data;
  u64   size;
};

INLINE String
str_lit(const char *c)
{
  String result;
  result.data = (char *)c;
  result.size = strlen(c);
  return result;
}

INLINE b32
str_eq(const String left, const String right)
{
  b32 result = !strcmp(left.data, right.data);
  return result;
}
