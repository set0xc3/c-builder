#include <cbased.h>

int
main(void)
{
  char cwd[256];

  if (chdir("../../") == 0) {
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
      printf("Родительский каталог: %s\n", cwd);
    } else {
      perror("Ошибка при получении родительского каталога");
      return 1;
    }
  }

  os_init(false);
  gfx_init();

  b32 is_quit = false;
  while (!is_quit) {
    SDL_Event event;
    while (os_event_next(&event)) {
      if (!os_process_event(&event)) {
        is_quit = true;
      }
    }

    gfx_frame_begin();
    gfx_quad_push(vec4_init(0, 0, 100, 100), vec4_init(1, 0, 1, 1));
    gfx_frame_end();
    os_window_swap_buffer(os_window_root_get());
    os_delay(1);
  }

  os_destroy();
  gfx_destroy();

  return 0;
}
