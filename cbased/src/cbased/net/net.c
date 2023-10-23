#include "cbased/net/net.h"
#include "cbased/core/base.h"
#include "cbased/core/log.h"

api b32
net_init(void)
{
  i32 error = -1;
#if defined(_WIN32)
  WSADATA d;
  if (WSAStartup(MAKEWORD(2, 2), &d)) {
    fprintf(stderr, "Failed to initialize.\n");
    return false;
  }
#endif

  return true;
}

api b32
net_destroy(void)
{
  return true;
}

api NET_Socket *
net_socket_create(u32 type)
{
  NET_Socket *result = (NET_Socket *)malloc(sizeof(NET_Socket));
  memset(result, 0, sizeof(NET_Socket));

  result->id = uuid_gen();

  i32 socket_handle = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_handle < 0) {
    LOG_ERR("Socket create failed\n");
    return NULL;
  }
  result->handle   = socket_handle;
  result->is_valid = true;

  return result;
}

api void
net_socket_destroy(NET_Socket *socket)
{
  if (socket == NULL) {
    LOG_ERR("Socket is nullptr\n");
    return;
  }
  close(socket->handle);
  free(socket);
}

api b32
net_socket_bind(NET_Socket *socket, const char *ip, u32 port)
{
  if (!socket->is_valid) {
    return false;
  }

  i32 opt = 1;

  if (setsockopt(socket->handle, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    LOG_ERR("Socket failed\n");
    return false;
  }

  socket->addr.sin_family      = AF_INET;
  socket->addr.sin_addr.s_addr = INADDR_ANY;
  socket->addr.sin_port        = htons(port);

  if (inet_pton(AF_INET, ip, &socket->addr.sin_addr) <= 0) {
    LOG_ERR("Invalid address/Address not supported\n");
    return false;
  }

  if (bind(socket->handle, (struct sockaddr *)&socket->addr,
           sizeof(socket->addr))
      < 0) {
    LOG_ERR("Socket bind failed\n");
    return false;
  }

  return true;
}

api b32
net_socket_connect(NET_Socket *socket, const char *ip, u32 port)
{
  if (!socket->is_valid) {
    return false;
  }

  socket->addr.sin_family = AF_INET;
  socket->addr.sin_port   = htons(port);

  if (inet_pton(AF_INET, "127.0.0.1", &socket->addr.sin_addr) <= 0) {
    LOG_ERR("Invalid address/Address not supported\n");
    return false;
  }
  if (connect(socket->handle, (struct sockaddr *)&socket->addr,
              sizeof(socket->addr))
      < 0) {
    LOG_ERR("Connection failed\n");
    return false;
  }

  socket->is_connect = true;

  return true;
}

api NET_Socket *
net_socket_accept(NET_Socket *socket_accept, u32 listeners)
{
  NET_Socket *result = malloc(sizeof(NET_Socket));
  memset(result, 0, sizeof(NET_Socket));

  result->id = uuid_gen();

  if (listen(socket_accept->handle, listeners) < 0) {
    LOG_ERR("Listen failed\n");
    return NULL;
  }

  socklen_t addr_len = sizeof(result->addr);
  result->handle
      = accept(socket_accept->handle, (struct sockaddr *)&result->addr,
               (socklen_t *)&addr_len);
  if (result->handle < 0) {
    return NULL;
  }

  result->is_valid = true;

  return result;
}

api void
net_socket_close(NET_Socket *socket)
{
  close(socket->handle);
  socket->is_connect = false;
}
