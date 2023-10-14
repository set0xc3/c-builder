#include <cbased.h>

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int
main(void)
{
  os_init(true);
  net_init();

  NET_Socket *server = net_socket_create(SOCK_STREAM);

  while (true) {
    if (!os_poll_event()) {
      break;
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

  return 0;
}
