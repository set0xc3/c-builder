#pragma once

#include "cbased/core/matrix.h"
#include "cbased/core/transform.h"
#include "cbased/core/vector.h"

typedef struct GFX_Camera GFX_Camera;

struct GFX_Camera {
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

api GFX_Camera *camera_create(f32 fov, f32 aspect, f32 near, f32 far);
api void        camera_destroy(GFX_Camera *camera);
api void        camera_reset(GFX_Camera *camera);

api mat4 camera_get_view_matrix(GFX_Camera *camera);

api void camera_resize_on(GFX_Camera *camera, vec2 size);
