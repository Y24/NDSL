#ifndef _SESSION_H
#define _SESSION_H 1
#include <unordered_map>
#include <vector>

#include "inet_addr.h"

class Session {
 private:
  std::unordered_map<InetAddr, int, InetAddrHash> fd;

 public:
  Session() = default;
  Session(std::vector<InetAddr> addrs, std::vector<int> fd);
  Session(InetAddr addr, int fd);
  bool insert(InetAddr addr, int fd);
  bool contains(InetAddr addr);
  bool merge(const Session& other);
  ~Session();
  bool isNull() const;
  std::unordered_map<InetAddr, int, InetAddrHash> getFd() const;
};
#endif // session.h
