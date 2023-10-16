#pragma once

#include "cbased/core/base.h"
#include "cbased/core/matrix.h"
#include "cbased/core/vector.h"
#include "cbased/os/os.h"

#define GFX_TRIANGLE_MAX 1024
#define GFX_VERTICES_MAX GFX_TRIANGLE_MAX * 3

typedef struct GFX_Vertex  GFX_Vertex;
typedef struct GFX_Shader  GFX_Shader;
typedef struct GFX_Context GFX_Context;

struct GFX_Vertex {
  vec2 position;
  vec4 color;
};

struct GFX_Shader {
  u32 handle;
};

struct GFX_Context {
  u32        vao;
  u32        vbo;
  GFX_Shader shader;

  mat4 projection;

  GFX_Vertex triangle_data[GFX_TRIANGLE_MAX];
  u32        triangle_count;
};

#ifdef __cplusplus
extern "C" {
#endif

API void gfx_init(void);
API void gfx_destroy(void);
API void gfx_frame_begin(void);
API void gfx_frame_end(void);
API void gfx_triangle_push(vec2 a, vec2 b, vec2 c, vec4 a_color, vec4 b_color,
                           vec4 c_color);
API void gfx_quad_push(rect quad, color tint);

#ifdef __cplusplus
}
#endif
