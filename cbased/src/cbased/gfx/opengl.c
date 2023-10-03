#include "opengl.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>

void
opengl_init(void)
{
  gladLoadGLLoader(SDL_GL_GetProcAddress);

  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void
opengl_destroy(void)
{
}

void
opengl_viewport_set(u32 x, u32 y, u32 width, u32 height)
{
  glViewport(x, y, width, height);
}

void
opengl_background_set(f32 r, f32 g, f32 b, f32 a)
{
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT);
}
