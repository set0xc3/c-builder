#pragma once

#include "cbased/core/base.h"
#include "cbased/core/vector.h"

#include <math.h>

#define MATH_PI       3.14159265358979323846f
#define MATH_INFINITY (1e30f * 1e30f)

function inline f32
deg_to_rad(f32 degree)
{
  return degree * (MATH_PI / 180.0f);
}

function inline f32
rad_to_deg(f32 radian)
{
  return radian * (180.0f / MATH_PI);
}

function inline u32
rgbu_to_u32(u32 r, u32 g, u32 b)
{
  return (((r & 0x0FF) << 16) | ((g & 0x0FF) << 8) | (b & 0x0FF));
}

function inline void
u32_to_rgb(u32 rgbu, u32 *out_r, u32 *out_g, u32 *out_b)
{
  *out_r = (rgbu >> 16) & 0x0FF;
  *out_g = (rgbu >> 8) & 0x0FF;
  *out_b = (rgbu)&0x0FF;
}

function inline vec3
rgb_u32_to_vec3(u32 r, u32 g, u32 b)
{
  vec3 out_v;
  out_v.r = r / 255.0f;
  out_v.g = g / 255.0f;
  out_v.b = b / 255.0f;
  return out_v;
}

function inline void
vec3_to_rgb_u32(vec3 v, u32 *out_r, u32 *out_g, u32 *out_b)
{
  *out_r = v.r * 255;
  *out_g = v.g * 255;
  *out_b = v.b * 255;
}
