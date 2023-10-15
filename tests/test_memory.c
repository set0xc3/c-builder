#include <cbased.h>

typedef struct Node Node;

struct Node {
  Uuid   uuid;
  String name;
};

int
main(void)
{
  MemoryArena arena = memory_arena_alloc_default();

  LOG_INFO("arena.memory: %ld\n", arena.memory);
  LOG_INFO("arena.max: %ld\n", arena.max);
  LOG_INFO("arena.pos: %ld\n", arena.pos);
  LOG_INFO("arena.commit_pos: %ld\n", arena.commit_pos);
  LOG_INFO("arena.align: %ld\n", arena.align);

  Node *node = MEMORY_PUSH_ARRAY_ZERO(&arena, Node, 1);
  node->uuid = uuid_gen();
  node->name = str_lit("Node");

  LOG_INFO("arena.memory: %ld\n", arena.memory);
  LOG_INFO("arena.max: %ld\n", arena.max);
  LOG_INFO("arena.pos: %ld\n", arena.pos);
  LOG_INFO("arena.commit_pos: %ld\n", arena.commit_pos);
  LOG_INFO("arena.align: %ld\n", arena.align);

  LOG_INFO("node.uuid: %s\n", node->uuid.value);
  LOG_INFO("node.name: %s\n", node->name.data);

  return 0;
}
