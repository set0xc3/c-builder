#include "core/base.h"
#include "core/container.h"
#include "core/intrinsics.h"

typedef struct Entity     Entity;
typedef struct EntityNode EntityNode;
typedef struct EntityList EntityList;

struct Entity {
  Uuid uuid;
};

struct EntityNode {
  IL_Node next;
  Entity  entity;
};

struct EntityList {
  IL_List list;
  u32     count;
};

int
main(void)
{
  LOG_INFO("[TEST] [Intrusive]\n");

  EntityList entity_list = { 0 };

  for (size_t i = 0; i < 4; i++) {
    EntityNode *node  = malloc(sizeof(EntityNode));
    node->entity.uuid = uuid_gen();
    IL_push_back(&entity_list.list, &node->next);
    entity_list.count++;
  }

  IL_pop_back(&entity_list.list);
  entity_list.count--;

  LOG_INFO("EntityList Count: %d\n", entity_list.count);

  IL_Iterator it = IL_iterator_head(entity_list.list, EntityNode, next);
  for (EntityNode *node = IL_iterate_next(&it); node != NULL;
       node             = IL_iterate_next(&it)) {
    Entity *entity = &node->entity;
    LOG_INFO("entity.uuid: %s\n", entity->uuid.value);
  }

  return 0;
}