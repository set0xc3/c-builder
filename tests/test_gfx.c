#include <cbased.h>

#include "cbased/gfx/camera.h"

int
main(void)
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

  f32         fov         = 45;
  f32         aspect      = 1280.0f / 720.0f;
  f32         near        = -0.01f;
  f32         far         = 1.0f;
  GFX_Camera *camera_view = camera_create(fov, aspect, near, far);

  while (!is_quit) {

    while (os_event_next(&event)) {
      if (!os_process_event(&event)) {
        is_quit = true;
      }
      viewport = os_window_root_get()->rect;
      camera_resize_on(camera_view, viewport.xy);
    }

    gfx_frame_begin();

    mat4 projection_matrix = camera_view->perspective_matrix;
    mat4 view_matrix       = camera_view->view_matrix;

    vec3 model_pos    = vec3_init(0.0f, 0.0f, 0.0f);
    mat4 model_matrix = mat4_identity();
    model_matrix      = mat4_translation(model_pos);

    gfx_shader_use(shader_default);
    gfx_shader_mat4_set(shader_default, str_lit("u_model"), model_matrix);
    gfx_shader_mat4_set(shader_default, str_lit("u_view"), view_matrix);
    gfx_shader_mat4_set(shader_default, str_lit("u_projection"),
                        projection_matrix);

    gfx_quad_push(vec4_init(0, 0, 100, 100), vec4_init(1, 0, 1, 1));

    gfx_frame_end();
    os_window_swap_buffer(os_window_root_get());
    os_delay(1);
  }

  os_destroy();
  gfx_destroy();

  return 0;
}
