#include <unordered_map>
#include <vector>

#include "inet_addr.cpp"

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
Session::~Session() {}
