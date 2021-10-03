#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <cstring>
#include <string>
using std::string;
class SocketServer {
 private:
  string ip;
  int port;
  int fd;
  int waitings;
  sockaddr_in addr;

 public:
  SocketServer(string ip, int port, int waitings = 5);
  bool serve();
  ~SocketServer();
};

SocketServer::SocketServer(string ip, int port, int waitings = 5)
    : ip(ip), port(port), waitings(waitings) {
  fd = socket(AF_INET, SOCK_STREAM, 0);
  bzero(&addr, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_pton(AF_INET, ip.data(), &addr.sin_addr);
}
bool SocketServer::serve() {
  return bind(fd, (sockaddr*)&addr, sizeof(addr)) != -1 &&
         listen(fd, waitings) != -1;
}

SocketServer::~SocketServer() {
  close(fd);
  printf("SocketServer destruct !");
}
