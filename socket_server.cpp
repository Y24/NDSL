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

#include "server_epoll_manager.cpp"

class SocketServer {
 private:
  const char* ip;
  int port;
  int fd;
  int waitings;
  sockaddr_in addr;

 public:
  SocketServer(const char* ip, int port, int waitings = 5);
  bool init();
  void serve(int size, int nEvents);
  ~SocketServer();
};
SocketServer::SocketServer(const char* ip, int port, int waitings)
    : ip(ip), port(port), waitings(waitings) {
  fd = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_pton(AF_INET, ip, &addr.sin_addr);
}
bool SocketServer::init() {
  return bind(fd, (sockaddr*)&addr, sizeof(addr)) != -1 &&
         listen(fd, waitings) != -1;
}

void SocketServer::serve(int size, int nEvents) {
  EpollManager mangager(fd, size, nEvents);
  mangager.work();
}

SocketServer::~SocketServer() {
  close(fd);
  printf("SocketServer destruct !");
}
