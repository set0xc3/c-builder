#include "core/base.h"
#include "core/gfx.h"
#include "core/os.h"

int
main(void)
{
  LOG_INFO("[TEST] OpenGL:\n");
  LOG_INFO("[TEST]   version: legacy\n");

  os_init(false);
  opengl_init();

  DrawList draw_list = { 0 };
  draw_list._points  = malloc(1024 * sizeof(Point));

  while (os_poll_event()) {
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glLoadIdentity();

    glBegin(GL_LINES);
    {
      glColor3f(1, 0, 1);
      glVertex2f(0, 0);

      glColor3f(1, 0, 1);
      glVertex2f(1, 1);
    }
    glEnd();

    glPopMatrix();

    os_window_swap_buffer(os_window_root_get());

    os_delay(1);
  }

  opengl_destroy();
  os_destroy();

  return 0;
}
