#include "socket_productor.h"
SocketProductor::SocketProductor(std::string ip, int port)
    : ip(ip), port(port) {
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_pton(AF_INET, ip.data(), &addr.sin_addr);
}
void SocketProductor::config(std::string ip, int port) {
  this->ip = ip;
  this->port = port;
  addr.sin_port = htons(port);
  inet_pton(AF_INET, ip.data(), &addr.sin_addr);
}
int SocketProductor::produce() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  return connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
}

SocketProductor::~SocketProductor() {}
