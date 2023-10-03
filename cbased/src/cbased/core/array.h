#pragma once

#include "cbased/core/base.h"

#define ARRAY_DEFAULT_CAPACITY 1

typedef struct Array Array;

struct Array {
  void *data;
  u64   size;
  u64   capacity;
};

INLINE void *
_array_create(u64 capacity, u64 stride)
{
  return NULL;
}

INLINE void _array_destroy(void *array_ptr);

INLINE u64 _array_size(void *array_ptr);

INLINE u64 _array_capacity(void *array_ptr);

INLINE void *_array_at(void *array_ptr, u64 index, u64 stride);

INLINE void _array_push(void *array_ptr, void *value_ptr, u64 stride);

INLINE void _array_pop(void *array_ptr, u64 stride);

INLINE void _array_insert(void *array_ptr, u64 index, void *value_ptr);

INLINE void _array_delete(void *array_ptr, u64 index);

INLINE u64 _array_find(void *array_ptr, void *value_ptr);

#define array_create(type) _array_create(ARRAY_DEFAULT_CAPACITY, sizeof(type))

#define array_reserve(capacity, type) _array_create(capacity, sizeof(type))

#define array_destroy(array_ptr) _array_destroy(array_ptr)

#define array_size(array_ptr) _array_size(array_ptr)

#define array_capacity(array_ptr) _array_capacity(array_ptr)

#define array_at(array_ptr, index, type)                                      \
  _array_at(array_ptr, index, sizeof(type))

#define array_push(array_ptr, value_ptr, type)                                \
  _array_push(array_ptr, value_ptr, sizeof(type))

#define array_pop(array_ptr, type) _array_pop(array_ptr, sizeof(type))
