#include "core.h"
#include "os/os.h"
#include "renderer/renderer.h"

int
main(void)
{
  os_startup(false);
  renderer_startup();

  while (os_poll_event()) {
    os_delay(1);
  }

  os_shutdown();

  return 0;
}
