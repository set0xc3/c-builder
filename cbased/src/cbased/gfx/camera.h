#pragma once

#include "cbased/core/matrix.h"
#include "cbased/core/transform.h"
#include "cbased/core/vector.h"

typedef struct Camera Camera;

struct Camera {
  b32 is_dirty;

  Transform transform;
  Transform target;

  // FPS
  f32 pitch;
  f32 yaw;
  f32 roll;

  f32 fov;
  f32 aspect;
  f32 near;
  f32 far;

  mat4 orthographic_matrix;
  mat4 perspective_matrix;
  mat4 view_matrix;
};

api Camera *camera_create(f32 fov, f32 aspect, f32 near, f32 far);
api void    camera_destroy(Camera *camera);
api void    camera_reset(Camera *camera);

api mat4 camera_get_orthographic_matrix(Camera *camera);
api mat4 camera_get_perspective_matrix(Camera *camera);
api mat4 camera_get_view_matrix(Camera *camera);

api void camera_resize_on(Camera *camera, vec4 viewport);
