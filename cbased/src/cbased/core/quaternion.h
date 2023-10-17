#pragma once

#include "cbased/core/base.h"
#include "cbased/core/math.h"
#include "cbased/core/matrix.h"

typedef vec4 quat;

INLINE vec4
quat_identity(void)
{
  vec4 out_quat;
  out_quat.w = 1.0f;
  return out_quat;
}

INLINE f32
quat_normal(vec4 q)
{
  return sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
}

INLINE vec4
quat_normalize(vec4 q)
{
  vec4 out_quat;
  f32  normal = quat_normal(q);
  out_quat.x  = q.x / normal;
  out_quat.y  = q.y / normal;
  out_quat.z  = q.z / normal;
  out_quat.w  = q.w / normal;
  return out_quat;
}

INLINE vec4
quat_conjugate(vec4 q)
{
  vec4 out_quat;
  out_quat.x = -q.x;
  out_quat.y = -q.y;
  out_quat.z = -q.z;
  out_quat.w = q.w;
  return out_quat;
}

INLINE vec4
quat_inverse(vec4 q)
{
  return quat_normalize(quat_conjugate(q));
}

INLINE vec4
quat_mul(vec4 a, vec4 b)
{
  vec4 out_quat;
  out_quat.x = a.x * b.w + a.y * b.z - a.z * b.y + a.w * b.x;
  out_quat.y = -a.x * b.z + a.y * b.w + a.z * b.x + a.w * b.y;
  out_quat.z = a.x * b.y - a.y * b.x + a.z * b.w + a.w * b.z;
  out_quat.w = -a.x * b.x - a.y * b.y - a.z * b.z + a.w * b.w;
  return out_quat;
}

INLINE f32
quat_dot(vec4 a, vec4 b)
{
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

// https://stackoverflow.com/questions/1556260/convert-quaternion-rotation-to-rotation-matrix
INLINE mat4
quat_to_mat4(vec4 q)
{
  mat4 out_matrix     = mat4_identity();
  vec4 n              = quat_normalize(q);
  out_matrix.data[0]  = 1.0f - 2.0f * n.y * n.y - 2.0f * n.z * n.z;
  out_matrix.data[1]  = 2.0f * n.x * n.y - 2.0f * n.z * n.w;
  out_matrix.data[2]  = 2.0f * n.x * n.z + 2.0f * n.y * n.w;
  out_matrix.data[4]  = 2.0f * n.x * n.y + 2.0f * n.z * n.w;
  out_matrix.data[5]  = 1.0f - 2.0f * n.x * n.x - 2.0f * n.z * n.z;
  out_matrix.data[6]  = 2.0f * n.y * n.z - 2.0f * n.x * n.w;
  out_matrix.data[8]  = 2.0f * n.x * n.z - 2.0f * n.y * n.w;
  out_matrix.data[9]  = 2.0f * n.y * n.z + 2.0f * n.x * n.w;
  out_matrix.data[10] = 1.0f - 2.0f * n.x * n.x - 2.0f * n.y * n.y;
  return out_matrix;
}

INLINE mat4
quat_to_rotation_matrix(vec4 q, vec3 center)
{
  mat4 out_matrix;
  f32 *o = out_matrix.data;
  o[0]   = (q.x * q.x) - (q.y * q.y) - (q.z * q.z) + (q.w * q.w);
  o[1]   = 2.0f * ((q.x * q.y) + (q.z * q.w));
  o[2]   = 2.0f * ((q.x * q.z) - (q.y * q.w));
  o[3]   = center.x - center.x * o[0] - center.y * o[1] - center.z * o[2];
  o[4]   = 2.0f * ((q.x * q.y) - (q.z * q.w));
  o[5]   = -(q.x * q.x) + (q.y * q.y) - (q.z * q.z) + (q.w * q.w);
  o[6]   = 2.0f * ((q.y * q.z) + (q.x * q.w));
  o[7]   = center.y - center.x * o[4] - center.y * o[5] - center.z * o[6];
  o[8]   = 2.0f * ((q.x * q.z) + (q.y * q.w));
  o[9]   = 2.0f * ((q.y * q.z) - (q.x * q.w));
  o[10]  = -(q.x * q.x) - (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
  o[11]  = center.z - center.x * o[8] - center.y * o[9] - center.z * o[10];
  o[12]  = 0.0f;
  o[13]  = 0.0f;
  o[14]  = 0.0f;
  o[15]  = 1.0f;
  return out_matrix;
}

INLINE vec4
quat_from_axis_angle(vec3 axis, f32 angle, b32 normalize)
{
  const f32 half_angle = 0.5f * angle;
  f32       s          = sinf(half_angle);
  f32       c          = cosf(half_angle);
  vec4      q;
  q.x = s * axis.x;
  q.y = s * axis.y;
  q.z = s * axis.z;
  q.w = c;
  if (normalize) {
    return quat_normalize(q);
  }
  return q;
}

INLINE vec4
quat_slerp(vec4 a, vec4 b, f32 percentage)
{
  vec4 out_quaternion;

  // Source: https://en.wikipedia.org/wiki/Slerp
  // Only unit quaternions are valid rotations.
  // Normalize to avoid undefined behavior.
  vec4 v0 = quat_normalize(a);
  vec4 v1 = quat_normalize(b);

  // Compute the cosine of the angle between the two vectors.
  f32 dot = quat_dot(v0, v1);

  // If the dot product is negative, slerp won't take
  // the shorter path. Note that v1 and -v1 are equivalent when
  // the negation is applied to all four components. Fix by
  // reversing one quaternion.
  if (dot < 0.0f) {
    v1.x = -v1.x;
    v1.y = -v1.y;
    v1.z = -v1.z;
    v1.w = -v1.w;
    dot  = -dot;
  }
  const f32 DOT_THRESHOLD = 0.9995f;
  if (dot > DOT_THRESHOLD) {
    // If the inputs are too close for comfort, linearly interpolate
    // and normalize the result.
    out_quaternion.x = v0.x + ((v1.x - v0.x) * percentage);
    out_quaternion.y = v0.y + ((v1.y - v0.y) * percentage);
    out_quaternion.z = v0.z + ((v1.z - v0.z) * percentage);
    out_quaternion.w = v0.w + ((v1.w - v0.w) * percentage);
    return quat_normalize(out_quaternion);
  }

  // Since dot is in range [0, DOT_THRESHOLD], acos is safe
  f32 theta_0   = acosf(dot); // theta_0 = angle between input vectors
  f32 theta     = theta_0 * percentage; // theta = angle between v0 and result
  f32 sin_theta = sinf(theta);          // compute this value only once
  f32 sin_theta_0 = sinf(theta_0);      // compute this value only once
  f32 s0          = cosf(theta)
           - dot * sin_theta
                 / sin_theta_0; // == sin(theta_0 - theta) / sin(theta_0)
  f32 s1           = sin_theta / sin_theta_0;
  out_quaternion.x = (v0.x * s0) + (v1.x * s1);
  out_quaternion.y = (v0.y * s0) + (v1.y * s1);
  out_quaternion.z = (v0.z * s0) + (v1.z * s1);
  out_quaternion.w = (v0.w * s0) + (v1.w * s1);
  return out_quaternion;
}
