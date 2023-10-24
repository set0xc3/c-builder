#include "os.h"

#include <time.h>

global OS_Context *os_ctx;

api b32
os_init(b32 console)
{
  os_ctx             = malloc(sizeof(OS_Context));
  os_ctx->is_console = console;

  // Initialization of time-based random number generator
  srand(time(NULL));

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    LOG_ERR("[SDL] Failed init sdl: %s\n", SDL_GetError());
    return false;
  }

  if (!os_ctx->is_console) {
    os_ctx->root_window
        = os_window_create(str_lit("Window"), vec4_init(0, 0, 1280, 720));
  }

  return true;
}

api b32
os_destroy(void)
{
  if (!os_ctx->is_console) {
    os_window_destroy(os_ctx->root_window);
  }

  SDL_Quit();

  return true;
}

api b32
os_event_next(SDL_Event *out_event)
{
  return SDL_PollEvent(out_event);
}

api b32
os_process_event(SDL_Event *event)
{
  switch (event->type) {
  case SDL_QUIT:
    return false;

  case SDL_WINDOWEVENT: {
    switch (event->window.event) {
    case SDL_WINDOWEVENT_SIZE_CHANGED:
      os_ctx->root_window->rect.width  = (f32)event->window.data1;
      os_ctx->root_window->rect.height = (f32)event->window.data2;
      break;

    case SDL_WINDOWEVENT_MOVED:
      os_ctx->root_window->rect.x = (f32)event->window.data1;
      os_ctx->root_window->rect.y = (f32)event->window.data2;
      break;
    }
  } break;

  case SDL_MOUSEMOTION:
    break;

  case SDL_MOUSEWHEEL:
    break;

  case SDL_MOUSEBUTTONDOWN:
  case SDL_MOUSEBUTTONUP: {
  } break;

  case SDL_KEYDOWN:
  case SDL_KEYUP: {
  } break;
  }

  return true;
}

api void
os_delay(u32 ms)
{
  SDL_Delay(ms);
}

api u64
os_perf_counter(void)
{
  return SDL_GetPerformanceCounter();
}

api u64
os_perf_frequency(void)
{
  return SDL_GetPerformanceFrequency();
}

api string
os_file_read(const string path)
{
  string result = { 0 };
  FILE  *file   = fopen(path.str, "r");

  if (file != 0) {
    fseek(file, 0, SEEK_END);
    result.size = ftell(file);
    result.str  = malloc(result.size + 1);
    fseek(file, 0, SEEK_SET);

    fread(result.str, 1, result.size, file);
    fclose(file);

    result.str[result.size - 1] = '\0';
  }
  return result;
}

api OS_Library
os_library_load(const char *path)
{
  OS_Library result = { 0 };

#ifdef OS_LINUX
  char *ext = "so";
#elif OS_WINDOWS
  char *ext = "dll";
#endif

  char *dot      = ".";
  u64   dot_size = strlen(dot);

  u64    size     = strlen(path) + strlen(ext) + dot_size;
  string path_ext = { alloca(size), size };
  strncpy((char *)path_ext.str, path, path_ext.size);

  strncat((char *)path_ext.str, dot, path_ext.size);
  strncat((char *)path_ext.str, ext, path_ext.size);

  result.handle = SDL_LoadObject((const char *)path_ext.str);
  if (result.handle == NULL) {
    LOG_ERR("[SDL] %s\n", SDL_GetError());
  }

  result.is_valid = true;

  return result;
}

api void
os_library_unload(OS_Library *library)
{
  SDL_UnloadObject(library->handle);
  free(library);
}

api void *
os_library_load_function(OS_Library *library, const char *name)
{
  return SDL_LoadFunction(library->handle, name);
}

api OS_Window *
os_window_create(string title, vec4 rect)
{
  OS_Window *out_window;
  out_window = malloc(sizeof(OS_Window));
  memset(out_window, 0, sizeof(OS_Window));

  out_window->title = title;
  out_window->rect  = rect;

  u32 window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
                     | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI;

  out_window->sdl.window = SDL_CreateWindow(
      out_window->title.str, out_window->rect.x, out_window->rect.y,
      out_window->rect.width, out_window->rect.height, window_flags);

  if (!out_window->sdl.window) {
    LOG_FATAL("[SDL] Failed create window\n");
  }

  out_window->sdl.gl_ctx = SDL_GL_CreateContext(out_window->sdl.window);

  if (!out_window->sdl.gl_ctx) {
    LOG_FATAL("[SDL] Failed create opengl context\n");
  }

  return out_window;
}

api b32
os_window_destroy(OS_Window *window)
{
  if (window) {
    SDL_DestroyWindow(window->sdl.window);
    free(window);
  }

  return true;
}

api void
os_window_swap_buffer(OS_Window *window)
{
  SDL_GL_SwapWindow(window->sdl.window);
}

api OS_Window *
os_window_root_get(void)
{
  return os_ctx->root_window;
}
