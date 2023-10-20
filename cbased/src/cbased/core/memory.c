#include "cbased/core/memory.h"
#include "cbased/core/base.h"
#include "cbased/core/log.h"

api MemoryArena
memory_arena_alloc(u64 cap)
{
  MemoryArena result;
  result.memory = malloc(cap);
  memset(result.memory, 0, cap);
  result.max        = cap;
  result.pos        = sizeof(MemoryArena);
  result.commit_pos = MEMORY_COMMIT_SIZE;
  result.align      = 8;
  return result;
}

api MemoryArena
memory_arena_alloc_default(void)
{
  return memory_arena_alloc(MEMORY_COMMIT_SIZE);
}

api void
memory_arena_release(MemoryArena *arena)
{
  assert(!arena);
  free(arena);
}

api void *
memory_arena_push(MemoryArena *arena, u64 size)
{
  if (arena->pos + size > arena->max) {
    LOG_FATAL("Handle out-of-memory");
  }
  u8 *pos = (u8 *)arena->memory + arena->pos;
  arena->pos += size;
  return pos;
}

api void *
memory_arena_push_zero(MemoryArena *arena, u64 size)
{
  void *memory = memory_arena_push(arena, size);
  if (memory == 0) {
    return 0;
  }
  memset(memory, 0, size);
  return memory;
}

api void *
memory_arena_pop(MemoryArena *arena, u64 size)
{
  if (arena->pos == 0) {
    LOG_FATAL("Handle out-of-memory");
  }
  arena->pos -= size;
  u8 *pos = (u8 *)arena->pos - size;
  return pos;
}

api void
memory_arena_clear(MemoryArena *arena)
{
  arena->pos = sizeof(MemoryArena);
}

api u64
memory_arena_get_offset(MemoryArena *arena)
{
  return arena->pos;
}

// ArenaTemp

api MemoryArenaTemp
memory_arena_temp_begin(MemoryArena *arena)
{
  MemoryArenaTemp result = { 0 };
  result.arena           = arena;
  result.offset          = arena->pos;
  return result;
}

api void
memory_arena_temp_end(MemoryArenaTemp temp)
{
  temp.arena->pos = temp.offset;
}

api MemoryArenaTemp
memory_arena_get_scratch(MemoryArena *arena)
{
  MemoryArenaTemp temp = { 0 };
  temp.arena           = arena;
  temp.offset          = arena->pos;
  return temp;
}
