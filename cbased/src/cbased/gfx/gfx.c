#include "gfx.h"

static GFX_Context *gfx;

static int buf_idx = 0;

void
gfx_init(void)
{
  if (gfx != NULL) {
    assert(false);
    return;
  }

  gfx = malloc(sizeof(GFX_Context));
  memset(gfx, 0, sizeof(GFX_Context));

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
gfx_clear(vec4 clr)
{
  gfx_flush();
}

void
gfx_present(void)
{
  OS_Window *window = os_window_root_get();
  gfx_flush();
}
