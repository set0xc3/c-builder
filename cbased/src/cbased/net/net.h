#pragma once

#include "cbased/core/base.h"
#include "cbased/core/uuid.h"

#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif

typedef struct NET_Socket NET_Socket;

struct NET_Socket {
  uuid        id;
  i32         handle;
  b32         is_valid;
  b32         is_connect;
  b32         is_listen;
  const char *ip;
  u32         port;

  struct sockaddr_in addr;
};

#ifdef __cplusplus
extern "C" {
#endif

api void        net_init(void);
api void        net_destroy(void);
api NET_Socket *net_socket_create(u32 type);
api void        net_socket_destroy(NET_Socket *socket);
api b32         net_socket_bind(NET_Socket *socket, const char *ip, u32 port);
api b32 net_socket_connect(NET_Socket *socket, const char *ip, u32 port);
api NET_Socket *net_socket_accept(NET_Socket *socket_accept, u32 listeners);
api void        net_socket_close(NET_Socket *socket);

#ifdef __cplusplus
}
#endif
