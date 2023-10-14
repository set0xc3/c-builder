#include <cbased.h>

int
main(void)
{
  os_init(false);
  os_delay(1000);
  os_destroy();

  return 0;
}
