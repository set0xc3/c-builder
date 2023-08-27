#include "base/defines.h"
#include "base/log.h"
#include "base/math.h"
#include "base/string.h"
#include "base/uuid.h"
#include "base/vector.h"
#include "platform/platform.h"
#include "renderer/renderer.h"

#include <microui.h>

int
main(void)
{
    platform_startup(false);
    renderer_init();

    while (platform_poll_event())
    {
        renderer_clear(vec4_init(255.0f, 255.0f, 255.0f, 255.0f));
        renderer_present();

        platform_delay(1);
    }

    platform_shutdown();

    return 0;
}
