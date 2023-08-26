#pragma once

// Base Types

#include <assert.h>
#include <memory.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float  f32;
typedef double f64;

typedef i8  b8;
typedef i16 b16;
typedef i32 b32;
typedef i64 b64;

#define KB(value) (value << 10)
#define MB(value) (value << 20)
#define GB(value) ((u64)(value) << 30)
#define TB(value) ((u64)(value) << 40)

#define ArrayCount(a) (sizeof((a)) / sizeof(*(a)))

#define Clamp(value, min, max)                                                \
    ((value <= min) ? min : (value >= max) ? max : value)
#define ClampTop(value, max) ((value >= max) ? max : value)

// Platform Types

#ifdef __linux__
#define LINUX
#elif defined(__WIN32__)
#define WINDOWS
#endif

#ifdef EXPORT
#ifdef _MSC_VER
#define API __attribute__((dllexport))
#else
#define API __attribute__((visibility("default")))
#endif // _MSC_VER
#else  // Import
#ifdef _MSC_VER
#define API __attribute__((dllimport))
#else
#define API
#endif // _MSC_VER
#endif // EXPORT

#if defined(__clang__) || defined(__gcc__)
#define INLINE   __attribute__((always_inline)) inline
#define NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#define INLINE   __forceinline
#define NOINLINE __declspec(noinline)
#else
#define INLINE static inline
#define NOINLINE
#endif
