#pragma once

#include "core/base.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

typedef struct OS_Library {
  Uuid  uuid;
  b32   is_valid;
  void *handle;
} OS_Library;

typedef struct OS_Window {
  Uuid        uuid;
  const char *title;

  struct {
    SDL_Window   *window;
    SDL_GLContext gl_ctx;
  } sdl;

  struct {
    i32 x, y;
    i32 width, height;
  } rect;
} OS_Window;

typedef struct OS_Context {
  b32        is_console;
  OS_Window *root_window;
} OS_Context;

API b32 os_init(b32 console);
API b32 os_destroy(void);

API b32    os_poll_event(void);
API void   os_delay(u32 ms);
API u64    os_perf_counter(void);
API u64    os_perf_frequency(void);
API String os_file_read(const String path);

API OS_Library os_library_load(const char *path);
API void       os_library_unload(OS_Library *library);
API void      *os_library_load_function(OS_Library *library, const char *name);

API OS_Window *os_window_open(const char *title, i32 xpos, i32 ypos, i32 width,
                              i32 height);
API b32        os_window_close(OS_Window *window);
API void       os_window_swap_buffer(OS_Window *window);

API OS_Window *os_window_root_get(void);
