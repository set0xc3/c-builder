#include <cbased.h>

#include <unity.h>

typedef struct Entity     Entity;
typedef struct EntityNode EntityNode;
typedef struct EntityList EntityList;

struct Entity {
  uuid id;
};

struct EntityNode {
  DLL_Node node;
  Entity   entity;
};

struct EntityList {
  DLL_List list;
  u32      count;
};

void
setUp(void)
{
}

void
tearDown(void)
{
}

void
test_list(void)
{
  EntityList entity_list = { 0 };

  for (size_t i = 0; i < 4; i++) {
    EntityNode *node = malloc(sizeof(EntityNode));
    node->entity.id  = uuid_gen();
    DLL_push_back(&entity_list.list, &node->node);
    entity_list.count++;
  }

  DLL_pop_back(&entity_list.list);
  entity_list.count--;

  LOG_INFO("EntityList Count: %d\n", entity_list.count);

  DLL_Iterator it = DLL_iterator_head(entity_list.list, EntityNode, node);
  for (EntityNode *node = DLL_iterate_next(&it); node != NULL;
       node             = DLL_iterate_next(&it)) {
    Entity *entity = &node->entity;
    LOG_INFO("entity.uuid: %s\n", entity->id.value);
  }
}
