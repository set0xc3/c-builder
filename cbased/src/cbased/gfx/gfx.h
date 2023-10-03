#pragma once

#include "cbased/core/base.h"
#include "cbased/core/vector.h"
#include "cbased/os/os.h"

typedef struct DrawVertex DrawVertex;
typedef struct DrawCmd    DrawCmd;
typedef struct DrawList   DrawList;

struct DrawVertex {
  Vector3 position;
};

struct DrawCmd {
  u32 vtx_offset;
  u32 vtx_count;

  u32 idx_offset;
  u32 idx_count;
};

struct DrawList {
  void *vtx_buffers;
  u32   vtx_buffer_count;

  void *color_buffers;
  u32   color_buffer_count;

  void *idx_buffers;
  u32   idx_buffer_count;

  DrawCmd *cmd_buffers;
  u32      cmd_buffer_count;
};

#ifdef __cplusplus
extern "C" {
#endif

API void gfx_init(void);
API void gfx_destroy(void);
API void gfx_begin(u32 x, u32 y, u32 width, u32 height);
API void gfx_background_set(f32 r, f32 g, f32 b, f32 a);
API void gfx_flush(void);
API void gfx_clear(Vector4 color);
API void gfx_present(void);

API void gfx_draw_line(DrawList *draw_list, const Vector2 p1, const Vector2 p2,
                       const Color color, const f32 thickness);

#ifdef __cplusplus
}
#endif
