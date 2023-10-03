#pragma once

#include "cbased/core/base.h"

typedef struct Uuid Uuid;

struct Uuid {
  char value[36];
};

#ifdef __cplusplus
extern "C" {
#endif

API Uuid uuid_gen(void);

#ifdef __cplusplus
}
#endif
