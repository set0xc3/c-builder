#pragma once

#include "cbased/core/base.h"

#define MEMORY_COMMIT_SIZE GB(4)

typedef struct MemoryArena     MemoryArena;
typedef struct MemoryArenaTemp MemoryArenaTemp;

struct MemoryArena {
  void *memory;
  u64   max;
  u64   pos;
  u64   commit_pos;
  u64   align;
};

#ifdef __cplusplus
extern "C" {
#endif

API MemoryArena *arena_alloc(u64 cap);
API MemoryArena *arena_alloc_default(void);
API void         arena_release(MemoryArena *arena);
API void        *arena_push(MemoryArena *arena, u64 size);
API void        *arena_push_zero(MemoryArena *arena, u64 size);
API void        *arena_pop(MemoryArena *arena, u64 size);
API void         arena_clear(MemoryArena *arena);
API u64          arena_get_offset(MemoryArena *arena);

#ifdef __cplusplus
}
#endif

// Helpers

#define MEMORY_PUSH_ARRAY(arena, type, count)                                 \
  (type *)arena_push(arena, sizeof(type) * count)
#define MEMORY_PUSH_ARRAY_ZERO(arena, type, count)                            \
  (type *)arena_push_zero(arena, sizeof(type) * count)

// MemoryArenaTemp

struct MemoryArenaTemp {
  MemoryArena *arena;
  u64          offset;
};

#ifdef __cplusplus
extern "C" {
#endif

API MemoryArenaTemp arena_temp_begin(MemoryArena *arena);
API void            arena_temp_end(MemoryArenaTemp temp);

#ifdef __cplusplus
}
#endif
