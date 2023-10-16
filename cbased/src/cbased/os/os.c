#include "os.h"

#include <time.h>

static OS_Context *ctx;

b32
os_init(b32 console)
{
  ctx             = malloc(sizeof(OS_Context));
  ctx->is_console = console;

  // Initialization of time-based random number generator
  srand(time(NULL));

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    LOG_ERR("[SDL] Failed init sdl: %s\n", SDL_GetError());
    exit(1);
  }

  if (!ctx->is_console) {
    ctx->root_window = os_window_open("Window", 0, 0, 1280, 720);
  }

  return true;
}

b32
os_destroy(void)
{
  if (!ctx->is_console) {
    os_window_close(ctx->root_window);
  }

  SDL_Quit();

  return true;
}

b32
os_poll_event(void)
{
  SDL_Event raw_event = { 0 };

  while (SDL_PollEvent(&raw_event)) {
    switch (raw_event.type) {
    case SDL_QUIT:
      return false;

    case SDL_WINDOWEVENT: {
      if (raw_event.window.event == SDL_WINDOWEVENT_RESIZED) {
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
  }

  return true;
}

void
os_delay(u32 ms)
{
  SDL_Delay(ms);
}

u64
os_perf_counter(void)
{
  return SDL_GetPerformanceCounter();
}

u64
os_perf_frequency(void)
{
  return SDL_GetPerformanceFrequency();
}

string
os_file_read(const string path)
{
  string result = { 0 };
  FILE  *file   = fopen(path.data, "r");

  if (file != 0) {
    fseek(file, 0, SEEK_END);
    result.size = ftell(file);
    result.data = malloc(result.size + 1);
    fseek(file, 0, SEEK_SET);

    fread(result.data, 1, result.size, file);
    fclose(file);

    result.data[result.size - 1] = '\0';
  }
  return result;
}

OS_Library
os_library_load(const char *path)
{
  OS_Library result = { 0 };

#ifdef LINUX
  char *ext = "so";
#elif WINDOWS
  char *ext = "dll";
#endif

  char *dot      = ".";
  u64   dot_size = strlen(dot);

  u64    size     = strlen(path) + strlen(ext) + dot_size;
  string path_ext = { alloca(size), size };
  strncpy((char *)path_ext.data, path, path_ext.size);

  strncat((char *)path_ext.data, dot, path_ext.size);
  strncat((char *)path_ext.data, ext, path_ext.size);

  result.handle = SDL_LoadObject((const char *)path_ext.data);
  if (result.handle == NULL) {
    LOG_ERR("[SDL] %s\n", SDL_GetError());
  }

  result.is_valid = true;

  return result;
}

void
os_library_unload(OS_Library *library)
{
  SDL_UnloadObject(library->handle);
  free(library);
}

void *
os_library_load_function(OS_Library *library, const char *name)
{
  return SDL_LoadFunction(library->handle, name);
}

OS_Window *
os_window_open(const char *title, i32 xpos, i32 ypos, i32 width, i32 height)
{
  OS_Window *out_window;
  out_window = malloc(sizeof(OS_Window));
  memset(out_window, 0, sizeof(OS_Window));

  out_window->title       = title;
  out_window->rect.x      = xpos;
  out_window->rect.y      = ypos;
  out_window->rect.width  = width;
  out_window->rect.height = height;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

  u32 window_flags
      = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;

  out_window->sdl.window = SDL_CreateWindow(
      out_window->title, out_window->rect.x, out_window->rect.y,
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

b32
os_window_close(OS_Window *window)
{
  if (window) {
    SDL_DestroyWindow(window->sdl.window);
    free(window);
  }

  return true;
}

void
os_window_swap_buffer(OS_Window *window)
{
  SDL_GL_SwapWindow(window->sdl.window);
}

OS_Window *
os_window_root_get(void)
{
  return ctx->root_window;
}
