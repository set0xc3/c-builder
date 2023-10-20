#pragma once

#include "cbased/core/base.h"
#include "cbased/core/matrix.h"
#include "cbased/core/quaternion.h"
#include "cbased/core/vector.h"

typedef struct Transform {
  vec3              position;
  vec4              rotation;
  vec3              scale;
  mat4              local;
  struct Transform *parent;
  b32               is_dirty;
} Transform;

#ifdef __cplusplus
extern "C" {
#endif

api Transform transform_create(vec3 position, vec4 rotation, vec3 scale);

api Transform transform_from_position(vec3 position);
api Transform transform_from_rotation(vec4 rotation);
api Transform transform_from_scale(vec3 scale);

api void transform_translate(Transform *transform, vec3 translation);
api void transform_rotate(Transform *transform, vec4 rotation);
api void transform_scale(Transform *transform, vec3 scale);

api mat4 transform_local_get(Transform *transform);
api mat4 transform_world_get(Transform *transform);

api vec3 transform_position_get(const Transform *transform);
api void transform_position_set(Transform *transform, vec3 position);

api vec4 transform_rotation_get(const Transform *transform);
api void transform_rotation_set(Transform *transform, vec4 rotation);

api vec3 transform_scale_get(const Transform *transform);
api void transform_scale_set(Transform *transform, vec3 scale);

api Transform *transform_parent_get(Transform *transform);
api void       transform_parent_set(Transform *transform, Transform *parent);

#ifdef __cplusplus
}
#endif
