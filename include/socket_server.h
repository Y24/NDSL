#ifndef _SOCKET_SERVER_H
#define _SOCKET_SERVER_H 1
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>

#include "server_epoll_manager.h"

/// The Top class running in the server side.
class SocketServer {
 private:
  std::string ip;
  int port;
  int fd;
  int waitings;
  sockaddr_in addr;

 public:
  SocketServer(std::string ip, int port, int waitings = 5);
  bool init();
  void serve(int size, int nEvents);
  ~SocketServer();
};
#endif  // socket_server.h
