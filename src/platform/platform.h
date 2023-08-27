#pragma once

#include "base/defines.h"
#include "base/uuid.h"

#include <SDL2/SDL.h>

typedef struct Library
{
    Uuid  uuid;
    b8    is_valid;
    void *handle;
} Library;

typedef struct Window
{
    Uuid        uuid;
    const char *title;

    struct
    {
        SDL_Window   *handle;
        SDL_GLContext context;
        SDL_Texture  *texture;
    } sdl;

    struct
    {
        i32 x, y;
        i32 width, height;
    } rect;
} Window;

typedef struct PlatformState
{
    b8      is_console;
    Window *root_window;
} PlatformState;

API b8 platform_startup(b8 console);
API b8 platform_shutdown(void);

API b8   platform_poll_event(void);
API void platform_delay(u32 ms);
API u64  platform_perf_counter(void);
API u64  platform_perf_frequency(void);

API Library platform_library_load(const char *path);
API void    platform_library_unload(Library *library);
API void   *platform_library_load_function(Library *library, const char *name);

API Window *platform_window_open(const char *title, i32 xpos, i32 ypos,
                                 i32 width, i32 height);
API b8      platform_window_close(Window *window);
API void    platform_window_swap_buffer(Window *window);

API Window *platform_window_root_get(void);
