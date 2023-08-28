#include "base/defines.h"
#include "base/log.h"
#include "base/math.h"
#include "base/string.h"
#include "base/uuid.h"
#include "base/vector.h"
#include "os/os.h"
#include "renderer/renderer.h"

int
main(void)
{
  os_startup(false);
  renderer_init();

  // while (os_poll_event()) {
  //   os_delay(1);
  // }
  //
  // os_shutdown();

  return 0;
}
