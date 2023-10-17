#include "cbased/gfx/gfx.h"

#include <glad/glad.h>

static GFX_Context *gfx;

void
gfx_init(void)
{
  if (gfx != NULL) {
    assert(false);
    return;
  }

  gfx = malloc(sizeof(GFX_Context));
  memset(gfx, 0, sizeof(GFX_Context));

  gfx->projection = mat4_ortho(0.0f, 1080.0f, 720.0f, 0.0f, -0.01f, 1.0f);

  gladLoadGL();

  // Buffers
  glGenVertexArrays(1, &gfx->vao);
  glBindVertexArray(gfx->vao);

  glGenBuffers(1, &gfx->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, gfx->vbo);
  glBufferData(GL_ARRAY_BUFFER, GFX_VERTICES_MAX * sizeof(GFX_Vertex), NULL,
               GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GFX_Vertex),
                        (void *)offsetof(GFX_Vertex, position));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GFX_Vertex),
                        (void *)offsetof(GFX_Vertex, color));
  glEnableVertexAttribArray(1);

  // Shaders

  gfx->shader.handle = glCreateProgram();

  string vert_code = os_file_read(str_lit("assets/shaders/default.vert.glsl"));
  string frag_code = os_file_read(str_lit("assets/shaders/default.frag.glsl"));

  // LOG_INFO("%s\n", vert_code.str);
  // LOG_INFO("%s\n", frag_code.str);

  u32 vert_module = glCreateShader(GL_VERTEX_SHADER);
  u32 frag_module = glCreateShader(GL_FRAGMENT_SHADER);

  i32 vert_code_size = vert_code.size;
  i32 frag_code_size = frag_code.size;
  glShaderSource(vert_module, 1, (const GLchar *const *)&vert_code.str,
                 &vert_code_size);
  glShaderSource(frag_module, 1, (const GLchar *const *)&frag_code.str,
                 &frag_code_size);

  glCompileShader(vert_module);
  glCompileShader(frag_module);

  i32 error = -1;

  glGetShaderiv(vert_module, GL_COMPILE_STATUS, &error);
  if (error == GL_FALSE) {
    LOG_ERR("Vertex Shader Compilation failed!\n");
    i32 length = 0;
    glGetShaderiv(vert_module, GL_INFO_LOG_LENGTH, &length);

    GLchar *info = malloc(length * sizeof(GLchar));
    glGetShaderInfoLog(vert_module, length * sizeof(GLchar), NULL, info);
    LOG_ERR("%s", info);
  }

  glGetShaderiv(frag_module, GL_COMPILE_STATUS, &error);
  if (error == GL_FALSE) {
    LOG_ERR("Fragment Shader Compilation failed!\n");
    i32 length = 0;
    glGetShaderiv(frag_module, GL_INFO_LOG_LENGTH, &length);

    GLchar *info = malloc(length * sizeof(GLchar));
    glGetShaderInfoLog(frag_module, length * sizeof(GLchar), NULL, info);
    LOG_ERR("%s", info);
  }

  glAttachShader(gfx->shader.handle, vert_module);
  glAttachShader(gfx->shader.handle, frag_module);

  glLinkProgram(gfx->shader.handle);

  glGetProgramiv(gfx->shader.handle, GL_COMPILE_STATUS, &error);
  if (error == GL_FALSE) {
    LOG_ERR("Program Linking Failed:\n");
    i32 length = 0;
    glGetProgramiv(gfx->shader.handle, GL_INFO_LOG_LENGTH, &length);

    i8 *info = malloc(length * sizeof(GLchar));
    glGetProgramInfoLog(gfx->shader.handle, length, NULL, (GLchar *)info);
    LOG_ERR("%s", info);
  }

  glDetachShader(gfx->shader.handle, vert_module);
  glDetachShader(gfx->shader.handle, frag_module);
  glDeleteShader(vert_module);
  glDeleteShader(frag_module);

  glUseProgram(gfx->shader.handle);
  u32 proj_loc = glGetUniformLocation(gfx->shader.handle, "u_proj");
  glUniformMatrix4fv(proj_loc, 1, GL_FALSE, gfx->projection.data);
}

void
gfx_destroy(void)
{
  glDeleteBuffers(1, &gfx->vbo);
  glDeleteVertexArrays(1, &gfx->vao);

  glDeleteProgram(gfx->shader.handle);
}

void
gfx_frame_begin(void)
{
  gfx->triangle_count = 0;

  glClear(GL_COLOR_BUFFER_BIT);
}

void
gfx_frame_end(void)
{
  glUseProgram(gfx->shader.handle);
  glBindVertexArray(gfx->vao);
  glBindBuffer(GL_ARRAY_BUFFER, gfx->vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  gfx->triangle_count * 3 * sizeof(GFX_Vertex),
                  gfx->triangle_data);

  glDrawArrays(GL_TRIANGLES, 0, gfx->triangle_count * 3);
}

void
gfx_viewport_set(rect viewport)
{
  glViewport(viewport.x, viewport.y, viewport.w, viewport.h);
}

void
gfx_bg_color_set(color bg)
{
  glClearColor(bg.r, bg.g, bg.b, bg.a);
}

void
gfx_triangle_push(vec2 a, vec2 b, vec2 c, color a_color, color b_color,
                  color c_color)
{
  if (gfx->triangle_count == GFX_TRIANGLE_MAX) {
    gfx_frame_end();
    gfx_frame_begin();
  }

  gfx->triangle_data[gfx->triangle_count * 3 + 0].position = a;
  gfx->triangle_data[gfx->triangle_count * 3 + 0].color    = a_color;
  gfx->triangle_data[gfx->triangle_count * 3 + 1].position = b;
  gfx->triangle_data[gfx->triangle_count * 3 + 1].color    = b_color;
  gfx->triangle_data[gfx->triangle_count * 3 + 2].position = c;
  gfx->triangle_data[gfx->triangle_count * 3 + 2].color    = c_color;

  gfx->triangle_count++;
}

void
gfx_quad_push(rect quad, color tint)
{
  gfx_triangle_push(
      vec2_init(quad.x, quad.y), vec2_init(quad.x + quad.w, quad.y),
      vec2_init(quad.x + quad.w, quad.y + quad.h), tint, tint, tint);
  gfx_triangle_push(vec2_init(quad.x, quad.y),
                    vec2_init(quad.x + quad.w, quad.y + quad.h),
                    vec2_init(quad.x, quad.y + quad.h), tint, tint, tint);
}
