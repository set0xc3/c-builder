#include <cbased.h>

typedef struct Entity Entity;

struct Entity {
  Uuid uuid;
};

int
main(void)
{
  Array *entities = array_create(Entity);

  return 0;
}
