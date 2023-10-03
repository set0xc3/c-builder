#include "core/base.h"
#include "core/container.h"
#include "core/intrinsics.h"

typedef struct Entity Entity;

struct Entity {
  Uuid uuid;
};

int
main(void)
{
  LOG_INFO("[TEST] [Array]\n");

  Array *entities = array_create(Entity);

  return 0;
}
