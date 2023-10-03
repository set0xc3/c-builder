#pragma once

#include "defines.h"

typedef struct Uuid Uuid;

struct Uuid {
  char value[36];
};

Uuid uuid_gen(void);
