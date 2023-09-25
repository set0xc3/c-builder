#include "gfx.h"

#include "base/log.h"
#include "os/os.h"

#define BUFFER_SIZE 16384

static int buf_idx = 0;

void
gfx_init(void)
{
}

void
gfx_destroy(void)
{
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
gfx_draw_add_line(DrawList *draw_list, const Vector2 p1, const Vector2 p2,
                  const Color color, const f32 thickness)
{
  Vector2 *pos = NULL;

  pos    = (Vector2 *)draw_list->_points + draw_list->_point_count;
  pos->x = p1.x;
  pos->y = p1.y;

  pos    = (Vector2 *)draw_list->_points + draw_list->_point_count + 1;
  pos->x = p2.x;
  pos->y = p2.y;
}
