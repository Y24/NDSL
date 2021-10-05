#include "inet_addr.h"
InetAddr::InetAddr(std::string ip, int port) : ip(ip), port(port) {}
bool InetAddr::operator==(const InetAddr& other) {
  return ip == other.ip && port == other.port;
}

std::size_t InetAddrHash::operator()(const InetAddr& addr) const {
  return std::hash<std::string>()(addr.ip) ^ std::hash<int>()(addr.port);
}
