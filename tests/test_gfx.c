#include <cbased.h>

int
main(void)
{
  os_init(false);
  gfx_init();

  os_destroy();
  gfx_destroy();

  return 0;
}
