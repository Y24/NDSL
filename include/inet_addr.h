#ifndef _INET_ADDR_H
#define _INET_ADDR_H 1
#include <string>
struct InetAddr {
  std::string ip;
  int port;
  InetAddr(std::string ip, int port);
  bool operator==(const InetAddr& other);
};
struct InetAddrHash {
  std::size_t operator()(const InetAddr& addr) const;
};
#endif  // inet_addr.h