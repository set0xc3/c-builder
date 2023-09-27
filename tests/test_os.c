#include "core/base.h"
#include "core/os.h"

int
main(void)
{
  LOG_INFO("[TEST] [OS]\n");

  os_init(false);
  os_delay(1000);
  os_destroy();

  return 0;
}
