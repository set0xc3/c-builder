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

api MemoryArena memory_arena_alloc(u64 cap);
api MemoryArena memory_arena_alloc_default(void);
api void        memory_arena_release(MemoryArena *arena);
api void       *memory_arena_push(MemoryArena *arena, u64 size);
api void       *memory_arena_push_zero(MemoryArena *arena, u64 size);
api void       *memory_arena_pop(MemoryArena *arena, u64 size);
api void        memory_arena_clear(MemoryArena *arena);
api u64         memory_arena_get_offset(MemoryArena *arena);

// Helpers

#define MEMORY_PUSH_ARRAY(arena, type, count)                                 \
  (type *)memory_arena_push(arena, sizeof(type) * count)
#define MEMORY_PUSH_ARRAY_ZERO(arena, type, count)                            \
  (type *)memory_arena_push_zero(arena, sizeof(type) * count)

// MemoryArenaTemp

struct MemoryArenaTemp {
  MemoryArena *arena;
  u64          offset;
};

api MemoryArenaTemp memory_arena_temp_begin(MemoryArena *arena);
api void            memory_arena_temp_end(MemoryArenaTemp temp);
