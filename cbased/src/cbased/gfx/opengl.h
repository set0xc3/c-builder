#pragma once

#include "cbased/core/base.h"

typedef struct OpenGLState {
  b32 is_init;
} OpenGLState;

#ifdef __cplusplus
extern "C" {
#endif

API void opengl_init(void);
API void opengl_destroy(void);
API void opengl_viewport_set(u32 x, u32 y, u32 width, u32 height);
API void opengl_background_set(f32 r, f32 g, f32 b, f32 a);

#ifdef __cplusplus
}
#endif
