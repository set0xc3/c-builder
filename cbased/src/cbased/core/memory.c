#include "cbased/core/memory.h"
#include "cbased/core/base.h"
#include "cbased/core/log.h"

MemoryArena *
arena_alloc(u64 cap)
{
  MemoryArena *result = malloc(sizeof(MemoryArena));
  memset(result, 0, sizeof(MemoryArena));

  result->memory     = result + sizeof(MemoryArena);
  result->max        = cap;
  result->pos        = sizeof(MemoryArena);
  result->commit_pos = MEMORY_COMMIT_SIZE;
  result->align      = 8;
  return result;
}

MemoryArena *
arena_alloc_default(void)
{
  return arena_alloc(MEMORY_COMMIT_SIZE);
}

void
arena_release(MemoryArena *arena)
{
  assert(!arena);

  free(arena);
}

void *
arena_push(MemoryArena *arena, u64 size)
{
  if (arena->pos + size > arena->max) {
    LOG_FATAL("Handle out-of-memory");
  }

  u8 *pos = (u8 *)arena->memory + arena->pos;
  arena->pos += size;
  return pos;
}

void *
arena_push_zero(MemoryArena *arena, u64 size)
{
  void *memory = arena_push(arena, size);
  if (memory == 0) {
    return 0;
  }

  memset(memory, 0, size);
  return memory;
}

void *
arena_pop(MemoryArena *arena, u64 size)
{
  if (arena->pos == 0) {
    LOG_FATAL("Handle out-of-memory");
  }

  arena->pos -= size;
  u8 *pos = (u8 *)arena->pos - size;
  return pos;
}

void
arena_clear(MemoryArena *arena)
{
  arena->pos = 0;
}

u64
arena_get_offset(MemoryArena *arena)
{
  return arena->pos;
}

// ArenaTemp

MemoryArenaTemp
arena_temp_begin(MemoryArena *arena)
{
  MemoryArenaTemp result = { 0 };
  result.arena           = arena;
  result.offset          = arena->pos;
  return result;
}

void
arena_temp_end(MemoryArenaTemp temp)
{
  temp.arena->pos = temp.offset;
}

MemoryArenaTemp
arena_get_scratch(MemoryArena *arena)
{
  MemoryArenaTemp temp = { 0 };
  temp.arena           = arena;
  temp.offset          = arena->pos;
  return temp;
}
