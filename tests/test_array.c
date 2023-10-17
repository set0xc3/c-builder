#include <cbased.h>

typedef struct Entity Entity;

struct Entity {
  uuid id;
};

int
main(void)
{
  Array *entities = array_create(Entity);

  return 0;
}
