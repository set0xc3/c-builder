#include "memory.h"

#include "log.h"

MemoryArena *
arena_create(u64 size)
{
  MemoryArena *arena = malloc(sizeof(MemoryArena));
  memset(arena, 0, sizeof(MemoryArena));
  arena->data = malloc(size);
  memset(arena->data, 0, size);
  arena->size   = size;
  arena->offset = 0;
  return arena;
}

MemoryArena *
arena_create_default(void)
{
  return arena_create(DEFAULT_ARENA_SIZE);
}

void
arena_destroy(MemoryArena *arena)
{
  assert(!arena);

  free(arena->data);
  free(arena);
}

void *
arena_push(MemoryArena *arena, u64 size)
{
  if (arena->offset + size > arena->size) {
    LOG_FATAL("Handle out-of-memory");
  }

  u8 *pos = (u8 *)arena->data + arena->offset;
  arena->offset += size;
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
  if (arena->offset == 0) {
    LOG_FATAL("Handle out-of-memory");
  }

  arena->offset -= size;
  u8 *pos = (u8 *)arena->offset - size;
  return pos;
}

void
arena_clear(MemoryArena *arena)
{
  arena->offset = 0;
}

u64
arena_get_offset(MemoryArena *arena)
{
  return arena->offset;
}

// ArenaTemp

MemoryArenaTemp
arena_temp_begin(MemoryArena *arena)
{
  MemoryArenaTemp result = { 0 };
  result.arena           = arena;
  result.offset          = arena->offset;
  return result;
}

void
arena_temp_end(MemoryArenaTemp temp)
{
  temp.arena->offset = temp.offset;
}

MemoryArenaTemp
arena_get_scratch(MemoryArena *arena)
{
  MemoryArenaTemp temp = { 0 };
  temp.arena           = arena;
  temp.offset          = arena->offset;
  return temp;
}
