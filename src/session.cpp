#include "session.h"
Session::Session(InetAddr addr, int fd) { this->fd[addr] = fd; }
Session::Session(std::vector<InetAddr> addrs, std::vector<int> fd) {
  for (int i = 0; i < addrs.size(); i++) {
    this->fd[addrs[i]] = fd[i];
  }
}
bool Session::contains(InetAddr addr) { return this->fd.count(addr); }
bool Session::insert(InetAddr addr, int fd) {
  if (this->fd.count(addr)) return false;
  this->fd[addr] = fd;
  return true;
}
bool Session::isNull() const {
  if (fd.empty()) return true;
  for (auto [_, i] : fd)
    if (i == -1) return true;
  return true;
}
bool Session::merge(const Session& other) {
  for (auto [addr, _] : other.getFd()) {
    if (this->contains(addr)) return false;
  }
  for (auto [addr, fd] : other.getFd()) {
    this->insert(addr, fd);
  }
  return true;
}
std::unordered_map<InetAddr, int, InetAddrHash> Session::getFd() const {
  return fd;
}
Session::Session() {}
Session::~Session() {}
