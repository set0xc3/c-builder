#pragma once

#include "cbased/core/base.h"

#include <math.h>

typedef union vec2 vec2;
typedef union vec3 vec3;
typedef union vec4 vec4;

union vec2 {
  struct {
    f32 x, y;
  };

  struct {
    f32 u, v;
  };

  struct {
    f32 width, height;
  };

  f32 elements[2];
};

union vec3 {
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
    vec2 xy;
    f32  _ignored0;
  };

  struct {
    f32  _ignored1;
    vec2 yz;
  };

  struct {
    vec2 uv;
    f32  _ignored2;
  };

  struct {
    f32  _ignored3;
    vec2 vw;
  };

  f32 elements[3];
};

union vec4 {
  struct {
    union {
      vec3 xyz;
      struct {
        f32 x, y, z;
      };
    };

    f32 w;
  };

  struct {
    union {
      vec3 rgb;
      struct {
        f32 r, g, b;
      };
    };

    f32 a;
  };

  struct {
    vec2 xy;
    f32  _ignored1;
    f32  _ignored2;
  };

  struct {
    f32  _ignored3;
    vec2 yz;
    f32  _ignored4;
  };

  struct {
    f32  _ignored5;
    f32  _ignored6;
    vec2 zw;
  };

  struct {
    f32 left, right, top, botton;
  };

  struct {
    f32 _ignored7;
    f32 _ignored8;
    f32 width;
    f32 height;
  };

  f32 elements[4];
};

inline vec2
vec2_init(f32 x, f32 y)
{
  vec2 v;
  v.x = x;
  v.y = y;
  return v;
}

inline vec2
vec2_add(vec2 left, vec2 right)
{
  vec2 v;
  v.x = left.x + right.x;
  v.y = left.y + right.y;
  return v;
}

inline vec2
vec2_addf(vec2 left, f32 right)
{
  vec2 v;
  v.x = left.x + right;
  v.y = left.y + right;
  return v;
}

inline vec2
vec2_sub(vec2 left, vec2 right)
{
  vec2 v;
  v.x = left.x - right.x;
  v.y = left.y - right.y;
  return v;
}

inline vec2
vec2_subf(vec2 left, f32 right)
{
  vec2 v;
  v.x = left.x - right;
  v.y = left.y - right;
  return v;
}

inline vec2
vec2_mul(vec2 left, vec2 right)
{
  vec2 v;
  v.x = left.x * right.x;
  v.y = left.y * right.y;
  return v;
}

inline vec2
vec2_mul_scalar(vec2 a, f32 scalar)
{
  vec2 v;
  v.x = a.x * scalar;
  v.y = a.y * scalar;
  return v;
}

inline vec2
vec2_div(vec2 left, vec2 right)
{
  vec2 v;
  v.x = left.x / right.x;
  v.y = left.y / right.y;
  return v;
}

inline vec2
vec2_divf(vec2 left, f32 right)
{
  vec2 v;
  v.x = left.x / right;
  v.y = left.y / right;
  return v;
}

inline f32
vec2_dot(vec2 left, vec2 right)
{
  return (left.x * right.x) + (left.y * right.y);
}

inline b32
vec2_eq(vec2 left, vec2 right)
{
  return left.x == right.x && left.y == right.y;
}

inline vec2
vec2_normalized(vec2 v)
{
  return vec2_mul_scalar(v, 1.0f / sqrtf(vec2_dot(v, v)));
}

inline f32
vec2_sqrt_len(vec2 v)
{
  return vec2_dot(v, v);
}

inline f32
vec2_length(vec2 v)
{
  return sqrtf(vec2_sqrt_len(v));
}

inline vec2
vec2_lerp(vec2 left, vec2 right, f32 time)
{
  return vec2_add(vec2_mul_scalar(left, 1.0f - time),
                  vec2_mul_scalar(right, time));
}

inline f32
vec2_distance(vec2 a, vec2 b)
{
  vec2 d = vec2_init(a.x - b.x, a.y - b.y);
  return vec2_length(d);
}

inline vec2
vec2_zero(void)
{
  return vec2_init(0.0f, 0.0f);
}

inline vec2
vec2_one(void)
{
  return vec2_init(1.0f, 1.0f);
}

inline vec2
vec2_left(void)
{
  return vec2_init(-1.0f, 0.0f);
}

inline vec2
vec2_right(void)
{
  return vec2_init(1.0f, 0.0f);
}

inline vec2
vec2_up(void)
{
  return vec2_init(0.0f, 1.0f);
}

inline vec2
vec2_down(void)
{
  return vec2_init(0.0f, -1.0f);
}

inline vec3
vec3_init(f32 x, f32 y, f32 z)
{
  vec3 v;
  v.x = x;
  v.y = y;
  v.z = z;
  return v;
}

inline vec3
vec3_add(vec3 left, vec3 right)
{
  vec3 v;
  v.x = left.x + right.x;
  v.y = left.y + right.y;
  v.z = left.z + right.z;
  return v;
}

inline vec3
vec3_sub(vec3 left, vec3 right)
{
  vec3 v;
  v.x = left.x - right.x;
  v.y = left.y - right.y;
  v.z = left.z - right.z;
  return v;
}

inline vec3
vec3_mul(vec3 left, vec3 right)
{
  vec3 v;
  v.x = left.x * right.x;
  v.y = left.y * right.y;
  v.z = left.z * right.z;
  return v;
}

inline vec3
vec3_mul_scalar(vec3 a, f32 scalar)
{
  vec3 v;
  v.x = a.x * scalar;
  v.y = a.y * scalar;
  v.z = a.z * scalar;
  return v;
}

inline vec3
vec3_div(vec3 left, vec3 right)
{
  vec3 v;
  v.x = left.x / right.x;
  v.y = left.y / right.y;
  v.z = left.z / right.z;
  return v;
}

inline vec3
vec3_divf(vec3 left, f32 right)
{
  vec3 v;
  v.x = left.x / right;
  v.y = left.y / right;
  v.z = left.z / right;
  return v;
}

inline f32
vec3_dot(vec3 left, vec3 right)
{
  return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
}

inline vec3
vec3_cross(vec3 a, vec3 b)
{
  vec3 v;
  v.x = a.y * b.z - a.z * b.y;
  v.y = a.z * b.x - a.x * b.z;
  v.z = a.x * b.y - a.y * b.x;
  return v;
}

inline b32
vec3_eq(vec3 left, vec3 right)
{
  return left.x == right.x && left.y == right.y && left.z == right.z;
}

inline vec3
vec3_normalized(vec3 v)
{
  return vec3_mul_scalar(v, 1.0f / sqrtf(vec3_dot(v, v)));
}

inline f32
vec3_sqrt_len(vec3 v)
{
  return vec3_dot(v, v);
}

inline f32
vec3_length(vec3 v)
{
  return sqrtf(vec3_sqrt_len(v));
}

inline vec3
vec3_lerp(vec3 left, vec3 right, f32 time)
{
  return vec3_add(vec3_mul_scalar(left, 1.0f - time),
                  vec3_mul_scalar(right, time));
}

inline f32
vec3_distance(vec3 a, vec3 b)
{
  vec3 d = vec3_init(a.x - b.x, a.y - b.y, a.z - b.z);
  return vec3_length(d);
}

inline vec3
vec3_zero(void)
{
  return vec3_init(0.0f, 0.0f, 0.0f);
}

inline vec3
vec3_one(void)
{
  return vec3_init(1.0f, 1.0f, 1.0f);
}

inline vec3
vec3_left(void)
{
  return vec3_init(-1.0f, 0.0f, 0.0f);
}

inline vec3
vec3_right(void)
{
  return vec3_init(1.0f, 0.0f, 0.0f);
}

inline vec3
vec3_up(void)
{
  return vec3_init(0.0f, 1.0f, 0.0f);
}

inline vec3
vec3_down(void)
{
  return vec3_init(0.0f, -1.0f, 0.0f);
}

inline vec4
vec4_init(f32 x, f32 y, f32 z, f32 w)
{
  vec4 v;
  v.x = x;
  v.y = y;
  v.z = z;
  v.w = w;
  return v;
}

inline vec4
vec4_add(vec4 left, vec4 right)
{
  vec4 v;
  v.x = left.x + right.x;
  v.y = left.y + right.y;
  v.z = left.z + right.z;
  v.w = left.w + right.w;
  return v;
}

inline vec4
vec4_sub(vec4 left, vec4 right)
{
  vec4 v;
  v.x = left.x - right.x;
  v.y = left.y - right.y;
  v.z = left.z - right.z;
  v.w = left.w - right.w;
  return v;
}

inline vec4
vec4_mul(vec4 left, vec4 right)
{
  vec4 v;
  v.x = left.x * right.x;
  v.y = left.y * right.y;
  v.z = left.z * right.z;
  v.w = left.w * right.w;
  return v;
}

inline vec4
vec4_mul_scalar(vec4 a, f32 scalar)
{
  vec4 v;
  v.x = a.x * scalar;
  v.y = a.y * scalar;
  v.z = a.z * scalar;
  v.w = a.w * scalar;
  return v;
}

inline vec4
vec4_div(vec4 left, vec4 right)
{
  vec4 v;
  v.x = left.x / right.x;
  v.y = left.y / right.y;
  v.z = left.z / right.z;
  v.w = left.w / right.w;
  return v;
}

inline vec4
vec4_divf(vec4 left, f32 right)
{
  vec4 v;
  v.x = left.x / right;
  v.y = left.y / right;
  v.z = left.z / right;
  v.w = left.w / right;
  return v;
}

inline f32
vec4_dot(vec4 left, vec4 right)
{
  return (left.x * right.x) + (left.y * right.y) + (left.z * right.z)
         + (left.w * right.w);
}

inline b32
vec4_eq(vec4 left, vec4 right)
{
  return left.x == right.x && left.y == right.y && left.z == right.z
         && left.w == right.w;
}

inline vec4
vec4_normalized(vec4 v)
{
  return vec4_mul_scalar(v, 1.0f / sqrtf(vec4_dot(v, v)));
}

inline f32
vec4_sqrt_len(vec4 v)
{
  return vec4_dot(v, v);
}

inline f32
vec4_length(vec4 v)
{
  return sqrtf(vec4_sqrt_len(v));
}

inline vec4
vec4_lerp(vec4 left, vec4 right, f32 time)
{
  return vec4_add(vec4_mul_scalar(left, 1.0f - time),
                  vec4_mul_scalar(right, time));
}

inline f32
vec4_distance(vec4 a, vec4 b)
{
  vec4 d = vec4_init(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
  return vec4_length(d);
}

inline vec4
vec4_zero(void)
{
  return vec4_init(0.0f, 0.0f, 0.0f, 0.0f);
}

inline vec4
vec4_one(void)
{
  return vec4_init(1.0f, 1.0f, 1.0f, 1.0f);
}

inline vec4
vec4_left(void)
{
  return vec4_init(-1.0f, 1.0f, 1.0f, 1.0f);
}

inline vec4
vec4_right(void)
{
  return vec4_init(1.0f, 0.0f, 0.0f, 0.0f);
}

inline vec4
vec4_up(void)
{
  return vec4_init(0.0f, 1.0f, 0.0f, 0.0f);
}

inline vec4
vec4_down(void)
{
  return vec4_init(0.0f, -1.0f, 0.0f, 0.0f);
}
