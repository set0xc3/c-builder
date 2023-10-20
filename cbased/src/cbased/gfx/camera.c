#include "cbased/gfx/camera.h"

api GFX_Camera *
camera_create(f32 fov, f32 aspect, f32 near, f32 far)
{
  GFX_Camera *result = malloc(sizeof(GFX_Camera));
  memset(result, 0, sizeof(GFX_Camera));
  result->fov       = fov;
  result->near      = near;
  result->far       = far;
  result->transform = transform_default();
  result->orthographic_matrix
      = mat4_orthographic(0, 1280, 720, 0, result->near, result->far);
  result->perspective_matrix
      = mat4_perspective(deg_to_rad(fov), aspect, near, far);
  result->view_matrix = mat4_look_at(
      result->transform.position,
      vec3_add(result->transform.position, vec3_init(0.0f, 0.0f, -1.0f)),
      vec3_init(0.0f, 1.0f, 0.0f));
  return result;
}

api void
camera_destroy(GFX_Camera *camera)
{
  free(camera);
}

api void
camera_reset(GFX_Camera *camera)
{
  if (camera != NULL) {
  }
}

api mat4
camera_get_view_matrix(GFX_Camera *camera)
{
  if (camera->is_dirty) {
    mat4 rotation       = mat4_euler_xyz(camera->transform.rotation.x,
                                         camera->transform.rotation.y,
                                         camera->transform.rotation.z);
    mat4 translation    = mat4_translation(camera->transform.position);
    camera->view_matrix = mat4_mul(rotation, translation);
    camera->view_matrix = mat4_inverse(camera->view_matrix);
    camera->is_dirty    = false;
  }

  return camera->view_matrix;
}

api void
camera_resize_on(GFX_Camera *camera, vec2 size)
{
  f32 aspect      = size.width / size.height;
  f32 half_height = size.height / 2.0f;
  f32 half_width  = half_height * aspect;

  camera->aspect = aspect;
  camera->orthographic_matrix
      = mat4_orthographic(-half_width, half_width, -half_height, half_height,
                          camera->near, camera->far);
  camera->perspective_matrix = mat4_perspective(camera->fov, camera->aspect,
                                                camera->near, camera->far);
}
