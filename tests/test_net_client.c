#include <cbased.h>

#include <unity.h>

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

void
setUp(void)
{
}

void
tearDown(void)
{
}

void
test_client(void)
{
  os_init(true);
  net_init();

  NET_Socket *server = net_socket_create(SOCK_STREAM);

  b32 is_quit = false;
  while (!is_quit) {
    SDL_Event event;
    while (os_event_next(&event)) {
      if (!os_process_event(&event)) {
        is_quit = true;
      }
    }

    if (!server->is_connect) {
      net_socket_connect(server, "127.0.0.1", 8080);
    } else {
      char send_data[] = "Hello server";
      send(server->handle, send_data, strlen(send_data), 0);
      LOG_INFO("[send] %s\n", send_data);

      char buf[BUFFER_SIZE] = { 0 };
      i32  bytes_read       = recv(server->handle, buf, ARRAY_COUNT(buf), 0);

      if (bytes_read > 0) {
        LOG_INFO("[recv] %.*s\n", bytes_read, buf);
      } else if (bytes_read == -1) {
        LOG_ERR("Recv failed\n");
        net_socket_destroy(server);
        server = net_socket_create(SOCK_STREAM);
      }
    }

    os_delay(500);
  }
  net_socket_destroy(server);
}
