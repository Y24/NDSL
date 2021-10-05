#include "session_productor.h"
SessionProductor::SessionProductor(std::string ip, int port)
    : ip(ip), port(port), socketProductor(SocketProductor(ip, port)) {}
void SessionProductor::config(std::string, int port) {
  this->ip = ip;
  this->port = port;
  this->socketProductor.config(ip, port);
}
Session SessionProductor::produce() {
  return Session(InetAddr(this->ip, this->port), socketProductor.produce());
}
std::vector<Session> SessionProductor::produce(int num) {
  std::vector<Session> res;
  for (int i = 0; i < num; i++) {
    res.emplace_back(produce());
  }
  return res;
}
SessionProductor::~SessionProductor() {}
