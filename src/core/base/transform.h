#pragma once

#include "defines.h"
#include "matrix.h"
#include "quaternion.h"
#include "vector.h"

typedef struct Transform {
  Vector3           position;
  Vector4           rotation;
  Vector3           scale;
  Matrix4           local;
  struct Transform *parent;
  b32               is_dirty;
} Transform;

API Transform transform_create(Vector3 position, Vector4 rotation,
                               Vector3 scale);

API Transform transform_from_position(Vector3 position);
API Transform transform_from_rotation(Vector4 rotation);
API Transform transform_from_scale(Vector3 scale);

API void transform_translate(Transform *transform, Vector3 translation);
API void transform_rotate(Transform *transform, Vector4 rotation);
API void transform_scale(Transform *transform, Vector3 scale);

API Matrix4 transform_local_get(Transform *transform);
API Matrix4 transform_world_get(Transform *transform);

API Vector3 transform_position_get(const Transform *transform);
API void    transform_position_set(Transform *transform, Vector3 position);

API Vector4 transform_rotation_get(const Transform *transform);
API void    transform_rotation_set(Transform *transform, Vector4 rotation);

API Vector3 transform_scale_get(const Transform *transform);
API void    transform_scale_set(Transform *transform, Vector3 scale);

API Transform *transform_parent_get(Transform *transform);
API void       transform_parent_set(Transform *transform, Transform *parent);
