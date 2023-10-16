#pragma once

#include "cbased/core/base.h"

typedef struct uuid uuid;

struct uuid {
  char value[36];
};

#ifdef __cplusplus
extern "C" {
#endif

API uuid uuid_gen(void);

#ifdef __cplusplus
}
#endif
