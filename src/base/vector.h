#pragma once

#include "defines.h"

#include <math.h>

typedef union Vector2 {
  struct {
    f32 x, y;
  };

  struct {
    f32 u, v;
  };

  struct {
    f32 left, right;
  };

  struct {
    f32 width, height;
  };

  f32 elements[2];
} Vector2;

typedef union Vector3 {
  struct {
    f32 x, y, z;
  };

  struct {
    f32 u, v, w;
  };

  struct {
    f32 r, g, b;
  };

  struct {
    Vector2 xy;
    f32     _ignored0;
  };

  struct {
    f32     _ignored1;
    Vector2 yz;
  };

  struct {
    Vector2 uv;
    f32     _ignored2;
  };

  struct {
    f32     _ignored3;
    Vector2 vw;
  };

  f32 elements[3];
} Vector3;

typedef union Vector4 {
  struct {
    union {
      Vector3 xyz;
      struct {
        f32 x, y, z;
      };
    };

    f32 w;
  };

  struct {
    union {
      Vector3 rgb;
      struct {
        f32 r, g, b;
      };
    };

    f32 a;
  };

  struct {
    Vector2 xy;
    f32     _ignored0;
    f32     _ignored1;
  };

  struct {
    f32     _ignored2;
    Vector2 yz;
    f32     _ignored3;
  };

  struct {
    f32     _ignored4;
    f32     _ignored5;
    Vector2 zw;
  };

  struct {
    f32 left, right, top, botton;
  };

  struct {
    f32 _ignored6;
    f32 _ignored7;
    f32 width, height;
  };

  f32 elements[4];
} Vector4;

typedef Vector2 Point;
typedef Vector4 Color;

INLINE Vector2
vec2_init(f32 x, f32 y)
{
  Vector2 v;
  v.x = x;
  v.y = y;
  return v;
}

INLINE Vector2
vec2_add(Vector2 left, Vector2 right)
{
  Vector2 v;
  v.x = left.x + right.x;
  v.y = left.y + right.y;
  return v;
}

INLINE Vector2
vec2_addf(Vector2 left, f32 right)
{
  Vector2 v;
  v.x = left.x + right;
  v.y = left.y + right;
  return v;
}

INLINE Vector2
vec2_sub(Vector2 left, Vector2 right)
{
  Vector2 v;
  v.x = left.x - right.x;
  v.y = left.y - right.y;
  return v;
}

INLINE Vector2
vec2_subf(Vector2 left, f32 right)
{
  Vector2 v;
  v.x = left.x - right;
  v.y = left.y - right;
  return v;
}

INLINE Vector2
vec2_mul(Vector2 left, Vector2 right)
{
  Vector2 v;
  v.x = left.x * right.x;
  v.y = left.y * right.y;
  return v;
}

INLINE Vector2
vec2_mul_scalar(Vector2 a, f32 scalar)
{
  Vector2 v;
  v.x = a.x * scalar;
  v.y = a.y * scalar;
  return v;
}

INLINE Vector2
vec2_div(Vector2 left, Vector2 right)
{
  Vector2 v;
  v.x = left.x / right.x;
  v.y = left.y / right.y;
  return v;
}

INLINE Vector2
vec2_divf(Vector2 left, f32 right)
{
  Vector2 v;
  v.x = left.x / right;
  v.y = left.y / right;
  return v;
}

INLINE f32
vec2_dot(Vector2 left, Vector2 right)
{
  return (left.x * right.x) + (left.y * right.y);
}

INLINE b32
vec2_eq(Vector2 left, Vector2 right)
{
  return left.x == right.x && left.y == right.y;
}

INLINE Vector2
vec2_normalized(Vector2 v)
{
  return vec2_mul_scalar(v, 1.0f / sqrtf(vec2_dot(v, v)));
}

INLINE f32
vec2_sqrt_len(Vector2 v)
{
  return vec2_dot(v, v);
}

INLINE f32
vec2_length(Vector2 v)
{
  return sqrtf(vec2_sqrt_len(v));
}

INLINE Vector2
vec2_lerp(Vector2 left, Vector2 right, f32 time)
{
  return vec2_add(vec2_mul_scalar(left, 1.0f - time),
                  vec2_mul_scalar(right, time));
}

INLINE f32
vec2_distance(Vector2 a, Vector2 b)
{
  Vector2 d = vec2_init(a.x - b.x, a.y - b.y);
  return vec2_length(d);
}

INLINE Vector2
vec2_zero(void)
{
  return vec2_init(0.0f, 0.0f);
}

INLINE Vector2
vec2_one(void)
{
  return vec2_init(1.0f, 1.0f);
}

INLINE Vector2
vec2_left(void)
{
  return vec2_init(-1.0f, 0.0f);
}

INLINE Vector2
vec2_right(void)
{
  return vec2_init(1.0f, 0.0f);
}

INLINE Vector2
vec2_up(void)
{
  return vec2_init(0.0f, 1.0f);
}

INLINE Vector2
vec2_down(void)
{
  return vec2_init(0.0f, -1.0f);
}

INLINE Vector3
vec3_init(f32 x, f32 y, f32 z)
{
  Vector3 v;
  v.x = x;
  v.y = y;
  v.z = z;
  return v;
}

INLINE Vector3
vec3_add(Vector3 left, Vector3 right)
{
  Vector3 v;
  v.x = left.x + right.x;
  v.y = left.y + right.y;
  v.z = left.z + right.z;
  return v;
}

INLINE Vector3
vec3_sub(Vector3 left, Vector3 right)
{
  Vector3 v;
  v.x = left.x - right.x;
  v.y = left.y - right.y;
  v.z = left.z - right.z;
  return v;
}

INLINE Vector3
vec3_mul(Vector3 left, Vector3 right)
{
  Vector3 v;
  v.x = left.x * right.x;
  v.y = left.y * right.y;
  v.z = left.z * right.z;
  return v;
}

INLINE Vector3
vec3_mul_scalar(Vector3 a, f32 scalar)
{
  Vector3 v;
  v.x = a.x * scalar;
  v.y = a.y * scalar;
  v.z = a.z * scalar;
  return v;
}

INLINE Vector3
vec3_div(Vector3 left, Vector3 right)
{
  Vector3 v;
  v.x = left.x / right.x;
  v.y = left.y / right.y;
  v.z = left.z / right.z;
  return v;
}

INLINE Vector3
vec3_divf(Vector3 left, f32 right)
{
  Vector3 v;
  v.x = left.x / right;
  v.y = left.y / right;
  v.z = left.z / right;
  return v;
}

INLINE f32
vec3_dot(Vector3 left, Vector3 right)
{
  return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
}

INLINE Vector3
vec3_cross(Vector3 a, Vector3 b)
{
  Vector3 v = { 0 };

  v.x = a.y * b.z - a.z * b.y;
  v.y = a.z * b.x - a.x * b.z;
  v.z = a.x * b.y - a.y * b.x;

  return v;
}

INLINE b32
vec3_eq(Vector3 left, Vector3 right)
{
  return left.x == right.x && left.y == right.y && left.z == right.z;
}

INLINE Vector3
vec3_normalized(Vector3 v)
{
  return vec3_mul_scalar(v, 1.0f / sqrtf(vec3_dot(v, v)));
}

INLINE f32
vec3_sqrt_len(Vector3 v)
{
  return vec3_dot(v, v);
}

INLINE f32
vec3_length(Vector3 v)
{
  return sqrtf(vec3_sqrt_len(v));
}

INLINE Vector3
vec3_lerp(Vector3 left, Vector3 right, f32 time)
{
  return vec3_add(vec3_mul_scalar(left, 1.0f - time),
                  vec3_mul_scalar(right, time));
}

INLINE f32
vec3_distance(Vector3 a, Vector3 b)
{
  Vector3 d = vec3_init(a.x - b.x, a.y - b.y, a.z - b.z);
  return vec3_length(d);
}

INLINE Vector3
vec3_zero(void)
{
  return vec3_init(0.0f, 0.0f, 0.0f);
}

INLINE Vector3
vec3_one(void)
{
  return vec3_init(1.0f, 1.0f, 1.0f);
}

INLINE Vector3
vec3_left(void)
{
  return vec3_init(-1.0f, 0.0f, 0.0f);
}

INLINE Vector3
vec3_right(void)
{
  return vec3_init(1.0f, 0.0f, 0.0f);
}

INLINE Vector3
vec3_up(void)
{
  return vec3_init(0.0f, 1.0f, 0.0f);
}

INLINE Vector3
vec3_down(void)
{
  return vec3_init(0.0f, -1.0f, 0.0f);
}

INLINE Vector4
vec4_init(f32 x, f32 y, f32 z, f32 w)
{
  Vector4 v;
  v.x = x;
  v.y = y;
  v.z = z;
  v.w = w;
  return v;
}

INLINE Vector4
vec4_add(Vector4 left, Vector4 right)
{
  Vector4 v;
  v.x = left.x + right.x;
  v.y = left.y + right.y;
  v.z = left.z + right.z;
  v.w = left.w + right.w;
  return v;
}

INLINE Vector4
vec4_sub(Vector4 left, Vector4 right)
{
  Vector4 v;
  v.x = left.x - right.x;
  v.y = left.y - right.y;
  v.z = left.z - right.z;
  v.w = left.w - right.w;
  return v;
}

INLINE Vector4
vec4_mul(Vector4 left, Vector4 right)
{
  Vector4 v;
  v.x = left.x * right.x;
  v.y = left.y * right.y;
  v.z = left.z * right.z;
  v.w = left.w * right.w;
  return v;
}

INLINE Vector4
vec4_mul_scalar(Vector4 a, f32 scalar)
{
  Vector4 v;
  v.x = a.x * scalar;
  v.y = a.y * scalar;
  v.z = a.z * scalar;
  v.w = a.w * scalar;
  return v;
}

INLINE Vector4
vec4_div(Vector4 left, Vector4 right)
{
  Vector4 v;
  v.x = left.x / right.x;
  v.y = left.y / right.y;
  v.z = left.z / right.z;
  v.w = left.w / right.w;
  return v;
}

INLINE Vector4
vec4_divf(Vector4 left, f32 right)
{
  Vector4 v;
  v.x = left.x / right;
  v.y = left.y / right;
  v.z = left.z / right;
  v.w = left.w / right;
  return v;
}

INLINE f32
vec4_dot(Vector4 left, Vector4 right)
{
  return (left.x * right.x) + (left.y * right.y) + (left.z * right.z)
         + (left.w * right.w);
}

INLINE b32
vec4_eq(Vector4 left, Vector4 right)
{
  return left.x == right.x && left.y == right.y && left.z == right.z
         && left.w == right.w;
}

INLINE Vector4
vec4_normalized(Vector4 v)
{
  return vec4_mul_scalar(v, 1.0f / sqrtf(vec4_dot(v, v)));
}

INLINE f32
vec4_sqrt_len(Vector4 v)
{
  return vec4_dot(v, v);
}

INLINE f32
vec4_length(Vector4 v)
{
  return sqrtf(vec4_sqrt_len(v));
}

INLINE Vector4
vec4_lerp(Vector4 left, Vector4 right, f32 time)
{
  return vec4_add(vec4_mul_scalar(left, 1.0f - time),
                  vec4_mul_scalar(right, time));
}

INLINE f32
vec4_distance(Vector4 a, Vector4 b)
{
  Vector4 d = vec4_init(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
  return vec4_length(d);
}

INLINE Vector4
vec4_zero(void)
{
  return vec4_init(0.0f, 0.0f, 0.0f, 0.0f);
}

INLINE Vector4
vec4_one(void)
{
  return vec4_init(1.0f, 1.0f, 1.0f, 1.0f);
}

INLINE Vector4
vec4_left(void)
{
  return vec4_init(-1.0f, 1.0f, 1.0f, 1.0f);
}

INLINE Vector4
vec4_right(void)
{
  return vec4_init(1.0f, 0.0f, 0.0f, 0.0f);
}

INLINE Vector4
vec4_up(void)
{
  return vec4_init(0.0f, 1.0f, 0.0f, 0.0f);
}

INLINE Vector4
vec4_down(void)
{
  return vec4_init(0.0f, -1.0f, 0.0f, 0.0f);
}
