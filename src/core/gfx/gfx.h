#pragma once

#include "core/base.h"
#include "core/os.h"

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

API void gfx_init(void);
API void gfx_destroy(void);
API void gfx_flush(void);
API void gfx_clear(Vector4 color);
API void gfx_present(void);

API void gfx_draw_line(DrawList *draw_list, const Vector2 p1, const Vector2 p2,
                       const Color color, const f32 thickness);
