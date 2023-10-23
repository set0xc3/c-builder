#pragma once

// Base Types

#include <assert.h>
#include <memory.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

#define global        static
#define function      static
#define local_persist static

#define KB(value) (value << 10)
#define MB(value) (value << 20)
#define GB(value) ((u64)(value) << 30)
#define TB(value) ((u64)(value) << 40)

#define ARRAY_COUNT(a) (sizeof((a)) / sizeof(*(a)))

#define CLAMP(value, min, max)                                                \
  ((value <= min) ? min : (value >= max) ? max : value)
#define CLAMP_TOP(value, max) ((value >= max) ? max : value)

// Platform Types

#ifdef __cplusplus
#define LANG_CPP
#endif

#ifdef __linux__
#define OS_LINUX
#else
#ifdef __WIN32__
#define OS_WINDOWS
#endif
#endif

#ifdef LANG_CPP
#ifdef OS_WINDOWS
#define exported extern "C" __declspec(dllexport)
#else
#define exported extern "C"
#endif
#else
#ifdef OS_WINDOWS
#define exported __declspec(dllexport)
#else
#define exported
#endif
#endif

#ifdef LANG_CPP
#ifdef OS_WINDOWS
#define imported extern "C" __declspec(dllimport)
#else
#define imported extern "C"
#endif
#else
#ifdef OS_WINDOWS
#define imported __declspec(dllimport)
#else
#define imported
#endif
#endif

#ifdef LANG_CPP
#define api extern "C"
#else
#define api
#endif

#ifdef LANG_CPP
#ifdef OS_WINDOWS
#ifdef exported
#define api extern "C" __declspec(dllexport)
#else
#define api extern "C" __declspec(dllimport)
#endif
#else
#define api extern "C"
#endif
#else
#ifdef OS_WINDOWS
#define api __declspec(dllexport)
#else
#define api
#endif
#endif

#if defined(__clang__) || defined(__gcc__)
#define inline   __attribute__((always_inline)) inline
#define noinline __attribute__((noinline))
#else
#define inline static inline
#define noinline
#endif

#define type_has_field(type, field)      offsetof(type, field)
#define type_field_type(type, field)     typeof(((type *)0)->field)
#define offset_of_by_string(type, field) ((size_t)(&((type *)0)->field))
