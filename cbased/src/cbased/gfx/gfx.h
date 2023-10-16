#pragma once

#include "cbased/core/base.h"
#include "cbased/core/matrix.h"
#include "cbased/core/vector.h"
#include "cbased/gfx/opengl.h"
#include "cbased/os/os.h"

#define GFX_TRIANGLE_MAX 16384

typedef struct GFX_RenderVertex GFX_RenderVertex;
typedef struct GFX_Context      GFX_Context;

struct GFX_RenderVertex {
  vec2 position;
  vec4 color;
};

struct GFX_Context {
  u32 vao;
  u32 vbo;
  u32 shader;

  mat4 projection;

  GFX_RenderVertex triangle_data[GFX_TRIANGLE_MAX];
  u32              triangle_count;
};

#ifdef __cplusplus
extern "C" {
#endif

API void gfx_init(void);
API void gfx_destroy(void);
API void gfx_begin(u32 x, u32 y, u32 width, u32 height);
API void gfx_background_set(f32 r, f32 g, f32 b, f32 a);
API void gfx_flush(void);
API void gfx_clear(vec4 color);
API void gfx_present(void);

#ifdef __cplusplus
}
#endif
