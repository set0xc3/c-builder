#include <cbased.h>

#if 0
const u32 width  = 800;
const u32 height = 600;

#define BUFFER_SIZE 16384

static f32 vtx_buffers[BUFFER_SIZE * 8];
static f32 color_buffers[BUFFER_SIZE * 16];
static u32 idx_buffers[BUFFER_SIZE * 6];

static u32 object_count;

static void
flush(void)
{
  if (object_count == 0) {
    return;
  }

  object_count = 0;
}

static void
push_quad(vec2 dst, vec2 src, Color color)
{
  if (object_count == BUFFER_SIZE) {
    flush();
  }
}

int
main(void)
{
  LOG_INFO("[TEST] OpenGL:\n");
  LOG_INFO("[TEST]   version: legacy\n");

  os_init(false);
  gl_init();

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  while (os_poll_event()) {
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    os_window_swap_buffer(os_window_root_get());

    os_delay(1);
  }

  gl_destroy();
  os_destroy();

  return 0;
}
#else
int
main(void)
{
  return 0;
}
#endif
