#pragma once

#include "cbased/core/base.h"
#include "cbased/core/log.h"
#include "cbased/core/string.h"
#include "cbased/core/uuid.h"
#include "cbased/core/vector.h"

#include <SDL2/SDL.h>

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

#ifdef __cplusplus
extern "C" {
#endif

API b32 os_init(b32 console);
API b32 os_destroy(void);

API b32    os_event_next(SDL_Event *out_event);
API b32    os_process_event(SDL_Event *event);
API void   os_delay(u32 ms);
API u64    os_perf_counter(void);
API u64    os_perf_frequency(void);
API string os_file_read(const string path);

API OS_Library os_library_load(const char *path);
API void       os_library_unload(OS_Library *library);
API void      *os_library_load_function(OS_Library *library, const char *name);

API OS_Window *os_window_open(string title, vec4 rect);
API b32        os_window_close(OS_Window *window);
API void       os_window_swap_buffer(OS_Window *window);

API OS_Window *os_window_root_get(void);

#ifdef __cplusplus
}
#endif
