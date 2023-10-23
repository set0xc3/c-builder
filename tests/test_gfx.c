#include <cbased.h>

#include <unity.h>

#include "cbased/gfx/camera.h"

void
setUp(void)
{
}

void
tearDown(void)
{
}

void
test_gfx(void)
{
  local_persist SDL_Event event;
  local_persist vec4      viewport;
  local_persist b32       is_quit = false;

  chdir("../../"); // TODO: Remove this!

  os_init(false);
  gfx_init();

  GFX_Shader *shader_default
      = gfx_shader_create(str_lit("assets/shaders/default_vert.glsl"),
                          str_lit("assets/shaders/default_frag.glsl"));

  f32  fov               = 90;
  f32  aspect            = viewport.width / viewport.height;
  f32  near              = -1.0f;
  f32  far               = 1.0f;
  mat4 projection_matrix = mat4_orthographic(0.0f, 1280, 720, 0.0f, near, far);

  while (!is_quit) {

    while (os_event_next(&event)) {
      if (!os_process_event(&event)) {
        is_quit = true;
      }
      viewport = os_window_root_get()->rect;
    }

    gfx_frame_begin();

    mat4 model_matrix = mat4_identity();
    model_matrix      = mat4_translation(vec3_zero());

    gfx_shader_use(shader_default);
    gfx_shader_mat4_set(shader_default, str_lit("u_projection"),
                        projection_matrix);
    gfx_shader_mat4_set(shader_default, str_lit("u_model"), model_matrix);

    gfx_quad_push(vec4_init(0, 0, 100, 100), vec4_init(1, 0, 1, 1));

    gfx_frame_end();
    os_window_swap_buffer(os_window_root_get());
    os_delay(1);
  }

  os_destroy();
  gfx_destroy();
}
