#include "transform.h"

Transform
transform_create(Vector3 position, Vector4 rotation, Vector3 scale)
{
    Transform transform;

    transform.position = position;
    transform.rotation = rotation;
    transform.scale    = scale;
    transform.local    = mat4_identity();
    transform.parent   = 0;

    return transform;
}

Transform
transform_from_position(Vector3 position)
{
    Transform transform;

    transform.position = position;
    transform.rotation = quat_identity();
    transform.scale    = vec3_zero();
    transform.local    = mat4_identity();
    transform.parent   = 0;
    transform.is_dirty = true;

    return transform;
}

Transform
transform_from_rotation(Vector4 rotation)
{
    Transform transform;

    transform.position = vec3_zero();
    transform.rotation = rotation;
    transform.scale    = vec3_zero();
    transform.local    = mat4_identity();
    transform.parent   = 0;
    transform.is_dirty = true;

    return transform;
}

Transform
transform_from_scale(Vector3 scale)
{
    Transform transform;

    transform.position = vec3_zero();
    transform.rotation = quat_identity();
    transform.scale    = scale;
    transform.local    = mat4_identity();
    transform.parent   = 0;
    transform.is_dirty = true;

    return transform;
}

void
transform_translate(Transform *transform, Vector3 translation)
{
    transform->position = vec3_add(transform->position, translation);
    transform->is_dirty = true;
}

void
transform_rotate(Transform *transform, Vector4 rotation)
{
    transform->rotation = quat_mul(transform->rotation, rotation);
    transform->is_dirty = true;
}

void
transform_scale(Transform *transform, Vector3 scale)
{
    transform->scale    = vec3_mul(transform->scale, scale);
    transform->is_dirty = true;
}

Matrix4
transform_local_get(Transform *transform)
{
    if (transform)
    {
        if (transform->is_dirty)
        {
            Matrix4 tr         = mat4_mul(quat_to_mat4(transform->rotation),
                                           mat4_translation(transform->position));
            tr                  = mat4_mul(mat4_scale(transform->scale), tr);
            transform->local    = tr;
            transform->is_dirty = false;
        }

        return transform->local;
    }
    return mat4_identity();
}

Matrix4
transform_world_get(Transform *transform)
{
    if (transform)
    {
        Matrix4 local = transform_local_get(transform);
        if (transform->parent)
        {
            Matrix4 parent = transform_world_get(transform->parent);
            return mat4_mul(local, parent);
        }
        return local;
    }
    return mat4_identity();
}

Vector3
transform_position_get(const Transform *transform)
{
    return transform->position;
}

void
transform_position_set(Transform *transform, Vector3 position)
{
    transform->position = position;
    transform->is_dirty = true;
}

Vector4
transform_rotation_get(const Transform *transform)
{
    return transform->rotation;
}

void
transform_rotation_set(Transform *transform, Vector4 rotation)
{
    transform->rotation = rotation;
    transform->is_dirty = true;
}

Vector3
transform_scale_get(const Transform *transform)
{
    return transform->scale;
}

void
transform_scale_set(Transform *transform, Vector3 scale)
{
    transform->scale    = scale;
    transform->is_dirty = true;
}

Transform *
transform_parent_get(Transform *transform)
{
    return transform->parent;
}

void
transform_parent_set(Transform *transform, Transform *parent)
{
    transform->parent   = parent;
    transform->is_dirty = true;
}