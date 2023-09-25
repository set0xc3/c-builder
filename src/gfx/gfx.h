#pragma once

#include "base/defines.h"
#include "base/vector.h"

typedef struct DrawVertex DrawVertex;
typedef struct DrawCmd    DrawCmd;
typedef struct DrawList   DrawList;

struct DrawVertex {
  Vector2 position;
  Vector2 uv;
  Vector3 color;
};

struct DrawCmd {
  Vector4 clip_rect;  // 4*4
  u32     texture_id; // 4-8
  u32     vtx_offset; // 4
  u32     idx_offset; // 4
};

struct DrawList {
  DrawCmd *cmd_buffers;
  u32      cmd_buffer_count;

  u32 *idx_buffers;
  u32  idx_buffer_count;

  u32 *vtx_buffers;
  u32  vtx_buffer_count;

  Vector2 *_points;
  u32      _point_count;
};

API void gfx_init(void);
API void gfx_destroy(void);
API void gfx_flush(void);
API void gfx_clear(Vector4 color);
API void gfx_present(void);

API void gfx_draw_add_line(DrawList *draw_list, const Vector2 p1,
                           const Vector2 p2, const Color color,
                           const f32 thickness);
