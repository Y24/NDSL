#ifndef _SESSION_PRODUCTOR_H
#define _SESSION_PRODUCTOR_H 1
#include <string>
#include <utility>
#include <vector>

#include "session.cpp"
#include "socket_productor.cpp"
class SessionProductor {
 private:
  std::string ip;
  int port;
  SocketProductor socketProductor;

 public:
  SessionProductor(std::string ip, int port);
  void config(std::string ip, int port);
  Session produce();
  std::vector<Session> produce(int num);
  ~SessionProductor();
};
#endif // session_productor.h
