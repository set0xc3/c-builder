#include "cbased/gfx/backend/gl.h"

#if 0
#include <glad/glad.h>

void
gl_init(void)
{
  gladLoadGLLoader(SDL_GL_GetProcAddress);

  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void
gl_destroy(void)
{
}

void
gl_viewport_set(u32 x, u32 y, u32 width, u32 height)
{
  glViewport(x, y, width, height);
}

void
gl_bg_color_set(f32 r, f32 g, f32 b, f32 a)
{
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT);
}
#endif
