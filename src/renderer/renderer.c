#include "renderer.h"
#include "core/log.h"
#include "os/os.h"
#include "renderer/vulkan.h"

#include <SDL2/SDL.h>

#include <vulkan/vulkan.h>

#define BUFFER_SIZE 16384

static int buf_idx = 0;

void
renderer_startup(void)
{
  vulkan_init();
}

void
renderer_flush(void)
{
  if (buf_idx == 0) {
    return;
  }

  buf_idx = 0;
}

void
renderer_draw_rect(Vector4 rect, Vector4 color)
{
  renderer_quad_push(rect, vec4_init(125.0f, 68.0f, 3.0f, 3.0f), color);
}

void
renderer_set_clip_rect(Vector4 rect)
{
  renderer_flush();
}

void
renderer_clear(Vector4 clr)
{
  renderer_flush();
}

void
renderer_present(void)
{
  OS_Window *window = os_window_root_get();
  renderer_flush();
}

void
renderer_window_resize(Vector2 rect)
{
}

void
renderer_quad_push(Vector4 src, Vector4 dst, Vector4 color)
{
  if (buf_idx == BUFFER_SIZE) {
    renderer_flush();
  }
}
