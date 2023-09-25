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
