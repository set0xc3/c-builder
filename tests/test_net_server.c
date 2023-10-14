#include <cbased.h>

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080

int
main(void)
{
  os_init(true);
  net_init();

  u32 listen_max = 1000;

  NET_Socket *server = net_socket_create(SOCK_STREAM);
  net_socket_bind(server, "127.0.0.1", 8080);

  while (true) {
    if (!os_poll_event()) {
      break;
    }

    NET_Socket *new_client = net_socket_accept(server, 1);
    if (new_client->is_valid) {
      char buf[1024]  = { 0 };
      i32  bytes_read = recv(new_client->handle, buf, ArrayCount(buf), 0);

      if (bytes_read > 0) {
        LOG_INFO("[recv] %.*s\n", bytes_read, buf);
      } else if (bytes_read < 0) {
        LOG_ERR("Read failed\n");
      }

      char *send_data = "Hello all";
      send(new_client->handle, send_data, strlen(send_data), 0);
      LOG_INFO("[send] %s\n", send_data);
    }

    os_delay(500);
  }
  shutdown(server->handle, SHUT_RDWR);

  return 0;
}
