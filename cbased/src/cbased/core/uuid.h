#pragma once

#include "cbased/core/base.h"

typedef struct uuid uuid;

struct uuid {
  char value[36];
};

api uuid uuid_gen(void);
