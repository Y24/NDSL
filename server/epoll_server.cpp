#include "socket_server.h"
int main(int argc, char const *argv[]) {
  SocketServer server("127.0.0.1", 8786);
  if (!server.init()) {
    perror("SocketServer Init fails: ");
  }
  server.serve(1024, 100);
  return 0;
}
