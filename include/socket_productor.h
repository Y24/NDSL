#ifndef _SOCKET_PRODUCTOR_H
#define _SOCKET_PRODUCTOR_H 1
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#include <string>
class SocketProductor {
 private:
  std::string ip;
  int port;
  sockaddr_in addr;

 public:
  SocketProductor(std::string, int port);
  void config(std::string, int port);
  int produce();
  ~SocketProductor();
};
#endif // socket_productor.h
