#include "cbased/gfx/camera.h"

api Camera *
camera_create(f32 fov, f32 aspect, f32 near, f32 far)
{
  Camera *result = malloc(sizeof(Camera));
  memset(result, 0, sizeof(Camera));

  return result;
}

api void
camera_destroy(Camera *camera)
{
  free(camera);
}
