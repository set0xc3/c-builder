#pragma once

#include "base/defines.h"
#include "base/string.h"
#include "base/uuid.h"

#include <SDL2/SDL.h>

typedef struct OS_Library {
  Uuid  uuid;
  b8    is_valid;
  void *handle;
} OS_Library;

typedef struct OS_Window {
  Uuid        uuid;
  const char *title;

  struct {
    SDL_Window *handle;
  } sdl;

  struct {
    i32 x, y;
    i32 width, height;
  } rect;
} OS_Window;

typedef struct OS_Context {
  b8         is_console;
  OS_Window *root_window;
} OS_Context;

API b8 os_startup(b8 console);
API b8 os_shutdown(void);

API b8     os_poll_event(void);
API void   os_delay(u32 ms);
API u64    os_perf_counter(void);
API u64    os_perf_frequency(void);
API String os_file_read(const String path);

API OS_Library os_library_load(const char *path);
API void       os_library_unload(OS_Library *library);
API void      *os_library_load_function(OS_Library *library, const char *name);

API OS_Window *os_window_open(const char *title, i32 xpos, i32 ypos, i32 width,
                              i32 height);
API b8         os_window_close(OS_Window *window);
API void       os_window_swap_buffer(OS_Window *window);

API OS_Window *os_window_root_get(void);
