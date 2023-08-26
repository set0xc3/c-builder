#include "platform.h"

#include "base/log.h"
#include "base/string.h"

#include <SDL2/SDL.h>

#include <time.h>

static PlatformState *state;

b8
platform_startup(b8 console)
{
    state             = malloc(sizeof(PlatformState));
    state->is_console = console;

    // Initialization of time-based random number generator
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        LOG_ERROR("[SDL] Failed init sdl: %s\n", SDL_GetError());
        exit(1);
    }

    if (!state->is_console)
    {
        // Use OpenGL 4.6
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                            SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

        state->root_window = platform_window_open("Window", 0, 0, 1280, 720);
    }

    return true;
}

b8
platform_shutdown(void)
{
    if (!state->is_console)
    {
        platform_window_close(state->root_window);
    }

    SDL_Quit();

    return true;
}

b8
platform_poll_event(void)
{
    SDL_Event raw_event = { 0 };

    while (SDL_PollEvent(&raw_event))
    {
        switch (raw_event.type)
        {
        case SDL_QUIT:
            return false;

        case SDL_WINDOWEVENT:
        {
            if (raw_event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
            }
        }
        break;

        case SDL_MOUSEMOTION:
            break;

        case SDL_MOUSEWHEEL:
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
        {
        }
        break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
        {
        }
        break;
        }
    }

    return true;
}

void
platform_delay(u32 ms)
{
    SDL_Delay(ms);
}

u64
platform_perf_counter(void)
{
    return SDL_GetPerformanceCounter();
}

u64
platform_perf_frequency(void)
{
    return SDL_GetPerformanceFrequency();
}

Library
platform_library_load(const char *path)
{
    Library result = { 0 };

#ifdef LINUX
    char *ext = "so";
#elif WINDOWS
    char *ext = "dll";
#endif

    char *dot      = ".";
    u64   dot_size = strlen(dot);

    u64    size     = strlen(path) + strlen(ext) + dot_size;
    String path_ext = { alloca(size), size };
    strncpy((char *)path_ext.data, path, path_ext.size);

    strncat((char *)path_ext.data, dot, path_ext.size);
    strncat((char *)path_ext.data, ext, path_ext.size);

    result.handle = SDL_LoadObject((const char *)path_ext.data);
    if (result.handle == NULL)
    {
        LOG_ERROR("[SDL] %s\n", SDL_GetError());
    }

    result.is_valid = true;

    return result;
}

void
platform_library_unload(Library *library)
{
    SDL_UnloadObject(library->handle);
    free(library);
}

void *
platform_library_load_function(Library *library, const char *name)
{
    return SDL_LoadFunction(library->handle, name);
}

Window *
platform_window_open(const char *title, i32 xpos, i32 ypos, i32 width,
                     i32 height)
{
    Window *out_window;
    out_window = malloc(sizeof(Window));
    memset(out_window, 0, sizeof(Window));

    out_window->title       = title;
    out_window->rect.x      = xpos;
    out_window->rect.y      = ypos;
    out_window->rect.width  = width;
    out_window->rect.height = height;

    u32 window_flags
        = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
    out_window->sdl.handle = SDL_CreateWindow(
        out_window->title, out_window->rect.x, out_window->rect.y,
        out_window->rect.width, out_window->rect.height, window_flags);
    if (!out_window->sdl.handle)
    {
        LOG_ERROR("[SDL] Failed create window\n");
        free(out_window);
        return false;
    }

    out_window->sdl.context = SDL_GL_CreateContext(out_window->sdl.handle);
    SDL_GL_MakeCurrent(out_window->sdl.handle, out_window->sdl.context);

    return out_window;
}

b8
platform_window_close(Window *window)
{
    if (window)
    {
        SDL_DestroyWindow(window->sdl.handle);
        free(window);
    }

    return true;
}

void
platform_window_swap_buffer(Window *window)
{
    SDL_GL_SwapWindow(window->sdl.handle);
}
