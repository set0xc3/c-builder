#include "gfx.h"

#define BUFFER_SIZE 16384

static int buf_idx = 0;

void
gfx_init(void)
{
  opengl_init();
}

void
gfx_destroy(void)
{
}

void
gfx_begin(u32 x, u32 y, u32 width, u32 height)
{
  opengl_viewport_set(x, y, width, height);
}
void
gfx_background_set(f32 r, f32 g, f32 b, f32 a)
{
  opengl_background_set(r, g, b, a);
}

void
gfx_flush(void)
{
  if (buf_idx == 0) {
    return;
  }

  buf_idx = 0;
}

void
gfx_clear(Vector4 clr)
{
  gfx_flush();
}

void
gfx_present(void)
{
  OS_Window *window = os_window_root_get();
  gfx_flush();
}

void
gfx_draw_line(DrawList *draw_list, const Vector2 p1, const Vector2 p2,
              const Color color, const f32 thickness)
{
  DrawVertex vertices[2] = {
    { vec3_init(p1.x, p1.y, 0) },
    { vec3_init(p2.x, p2.y, 0) },
  };

  DrawCmd cmd = {
    .vtx_offset = draw_list->vtx_buffer_count,
    .vtx_count  = ARRAY_COUNT(vertices),
  };

  u8 *vtx_buffer = (u8 *)draw_list->vtx_buffers;
  memcpy(vtx_buffer + (cmd.vtx_offset * sizeof(DrawVertex)), vertices,
         sizeof(vertices));

  u8 *color_buffer = (u8 *)draw_list->color_buffers;
  memcpy(vtx_buffer + (cmd.vtx_offset * sizeof(DrawVertex)), vertices,
         sizeof(vertices));

  draw_list->vtx_buffer_count += cmd.vtx_count;

  draw_list->cmd_buffers[draw_list->cmd_buffer_count] = cmd;
  draw_list->cmd_buffer_count++;
}
