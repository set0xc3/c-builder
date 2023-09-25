#pragma once

#include "base/defines.h"

typedef struct OpenGLState {
  b32 is_init;
} OpenGLState;

API void opengl_init(void);
API void opengl_destroy(void);
