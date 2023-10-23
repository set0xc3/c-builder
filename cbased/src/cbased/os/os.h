#pragma once

#include "cbased/core/base.h"
#include "cbased/core/log.h"
#include "cbased/core/string.h"
#include "cbased/core/uuid.h"
#include "cbased/core/vector.h"

#undef function
#include <SDL2/SDL.h>
#define function

typedef struct OS_Library {
  uuid  id;
  b32   is_valid;
  void *handle;
} OS_Library;

typedef struct OS_Window {
  uuid   id;
  string title;

  struct {
    SDL_Window   *window;
    SDL_GLContext gl_ctx;
  } sdl;

  vec4 rect;
} OS_Window;

typedef struct OS_Context {
  b32        is_console;
  OS_Window *root_window;
} OS_Context;

api b32 os_init(b32 console);
api b32 os_destroy(void);

api b32    os_event_next(SDL_Event *out_event);
api b32    os_process_event(SDL_Event *event);
api void   os_delay(u32 ms);
api u64    os_perf_counter(void);
api u64    os_perf_frequency(void);
api string os_file_read(const string path);

api OS_Library os_library_load(const char *path);
api void       os_library_unload(OS_Library *library);
api void      *os_library_load_function(OS_Library *library, const char *name);

api OS_Window *os_window_create(string title, vec4 rect);
api b32        os_window_destroy(OS_Window *window);
api void       os_window_swap_buffer(OS_Window *window);
api OS_Window *os_window_root_get(void);
