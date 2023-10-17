#include <cbased.h>

typedef struct Node Node;

struct Node {
  uuid   id;
  string name;
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
  node->id   = uuid_gen();
  node->name = str_lit("Node");

  memory_arena_clear(&arena);

  Node *_node = MEMORY_PUSH_ARRAY_ZERO(&arena, Node, 1);
  _node->id   = uuid_gen();
  _node->name = str_lit("Node2");

  LOG_INFO("arena.memory: %ld\n", arena.memory);
  LOG_INFO("arena.max: %ld\n", arena.max);
  LOG_INFO("arena.pos: %ld\n", arena.pos);
  LOG_INFO("arena.commit_pos: %ld\n", arena.commit_pos);
  LOG_INFO("arena.align: %ld\n", arena.align);

  LOG_INFO("node.uuid: %s\n", node->id.value);
  LOG_INFO("node.name: %s\n", node->name.str);

  return 0;
}
