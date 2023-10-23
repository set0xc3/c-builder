#include "cbased/gfx/gfx.h"

#include <glad/glad.h>

global GFX_Context *gfx;

api GFX_Context *
gfx_get()
{
  return gfx;
}

api void
gfx_init(void)
{
  if (gfx != NULL) {
    assert(false);
    return;
  }

  gfx = malloc(sizeof(GFX_Context));
  memset(gfx, 0, sizeof(GFX_Context));

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
}

api void
gfx_destroy(void)
{
  glDeleteBuffers(1, &gfx->vbo);
  glDeleteVertexArrays(1, &gfx->vao);
}

api void
gfx_begin(void)
{
  vec4 viewport = os_window_root_get()->rect;

  gfx->triangle_count = 0;

  glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
  glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(0, 0, 0, 0);
}

api void
gfx_end(void)
{
  glBindVertexArray(gfx->vao);
  glBindBuffer(GL_ARRAY_BUFFER, gfx->vbo);
  glBufferSubData(GL_ARRAY_BUFFER, 0,
                  gfx->triangle_count * 3 * sizeof(GFX_Vertex),
                  gfx->triangle_data);

  glDrawArrays(GL_TRIANGLES, 0, gfx->triangle_count * 3);
}

api void
gfx_bg_color_set(vec4 bg)
{
  glClearColor(bg.r, bg.g, bg.b, bg.a);
}

api void
gfx_triangle_push(vec2 a, vec2 b, vec2 c, vec4 a_color, vec4 b_color,
                  vec4 c_color)
{
  if (gfx->triangle_count == GFX_TRIANGLE_MAX) {
    gfx_end();
    gfx_begin();
  }

  gfx->triangle_data[gfx->triangle_count * 3 + 0].position = a;
  gfx->triangle_data[gfx->triangle_count * 3 + 0].color    = a_color;
  gfx->triangle_data[gfx->triangle_count * 3 + 1].position = b;
  gfx->triangle_data[gfx->triangle_count * 3 + 1].color    = b_color;
  gfx->triangle_data[gfx->triangle_count * 3 + 2].position = c;
  gfx->triangle_data[gfx->triangle_count * 3 + 2].color    = c_color;

  gfx->triangle_count++;
}

api void
gfx_quad_push(vec4 quad, vec4 tint)
{
  gfx_triangle_push(
      vec2_init(quad.x, quad.y), vec2_init(quad.x + quad.width, quad.y),
      vec2_init(quad.x + quad.width, quad.y + quad.height), tint, tint, tint);
  gfx_triangle_push(vec2_init(quad.x, quad.y),
                    vec2_init(quad.x + quad.width, quad.y + quad.height),
                    vec2_init(quad.x, quad.y + quad.height), tint, tint, tint);
}

api GFX_Shader *
gfx_shader_create(string vert_path, string frag_path)
{
  GFX_Shader *result = malloc(sizeof(GFX_Shader));
  memset(result, 0, sizeof(GFX_Shader));

  result->handle = glCreateProgram();

  string vert_code = os_file_read(vert_path);
  string frag_code = os_file_read(frag_path);

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

  glAttachShader(result->handle, vert_module);
  glAttachShader(result->handle, frag_module);

  glLinkProgram(result->handle);

  glGetProgramiv(result->handle, GL_COMPILE_STATUS, &error);
  if (error == GL_FALSE) {
    LOG_ERR("Program Linking Failed:\n");
    i32 length = 0;
    glGetProgramiv(result->handle, GL_INFO_LOG_LENGTH, &length);

    i8 *info = malloc(length * sizeof(GLchar));
    glGetProgramInfoLog(result->handle, length, NULL, (GLchar *)info);
    LOG_ERR("%s", info);
  }

  glDetachShader(result->handle, vert_module);
  glDetachShader(result->handle, frag_module);
  glDeleteShader(vert_module);
  glDeleteShader(frag_module);

  return result;
}

api void
gfx_shader_destroy(GFX_Shader *shader)
{
  glDeleteProgram(shader->handle);
  free(shader);
}

api void
gfx_shader_use(GFX_Shader *shader)
{
  glUseProgram(shader->handle);
}

api void
gfx_shader_mat4_set(GFX_Shader *shader, string name, mat4 matrix)
{
  glUniformMatrix4fv(glGetUniformLocation(shader->handle, name.str), 1,
                     GL_FALSE, matrix.data);
}
