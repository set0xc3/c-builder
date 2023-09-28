#include "core/base.h"
#include "core/container.h"
#include "core/intrinsics.h"

typedef struct Entity     Entity;
typedef struct EntityList EntityList;

struct Entity {
  IL_Node node;

  Uuid uuid;
};

struct EntityList {
  IL_List list;
  u32     count;
};

int
main(void)
{
  LOG_INFO("[TEST] [Intrusive]\n");

  EntityList list = { 0 };

  for (size_t i = 0; i < 4; i++) {
    Entity *entity = malloc(sizeof(Entity));
    entity->uuid   = uuid_gen();
    IL_push_back(&list.list, &entity->node);
    list.count++;
  }

  LOG_INFO("EntityList Count: %d\n", list.count);

  IL_Iterator it = IL_iterator_head(list.list, Entity, node);
  for (Entity *entity = (Entity *)IL_iterate_next(&it);;
       entity         = (Entity *)IL_iterate_next(&it)) {
    LOG_INFO("entity.uuid: %s\n", entity->uuid.value);
  }

  return 0;
}
