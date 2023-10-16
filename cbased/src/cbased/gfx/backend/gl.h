#pragma once

#include "cbased/core/base.h"

typedef struct GL_Context GL_Context;

struct GL_Context {
  b32 is_init;
};

#ifdef __cplusplus
extern "C" {
#endif

API void gl_init(void);
API void gl_destroy(void);
API void gl_viewport_set(u32 x, u32 y, u32 width, u32 height);
API void gl_bg_color_set(f32 r, f32 g, f32 b, f32 a);

#ifdef __cplusplus
}
#endif
