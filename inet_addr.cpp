#include <string>
struct InetAddr {
  std::string ip;
  int port;
  InetAddr(std::string ip, int port) : ip(ip), port(port) {}
  bool operator==(const InetAddr& other) {
    return ip == other.ip && port == other.port;
  }
};
struct InetAddrHash {
  std::size_t operator()(const InetAddr& addr) const {
    return std::hash<std::string>()(addr.ip) ^ std::hash<int>()(addr.port);
  }
};
