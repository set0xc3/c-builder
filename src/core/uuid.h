#pragma once

#include "defines.h"

typedef struct Uuid
{
    char value[36];
} Uuid;

Uuid uuid_gen(void);
