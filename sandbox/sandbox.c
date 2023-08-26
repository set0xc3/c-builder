#include "base/defines.h"
#include "base/log.h"
#include "base/math.h"
#include "base/string.h"
#include "base/uuid.h"

#include "platform/platform.h"

int
main(void)
{
    platform_startup(false);

    while (platform_poll_event())
    {
        platform_delay(1);
    }

    platform_shutdown();

    return 0;
}
