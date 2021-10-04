#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "session.cpp"

/// TODO:

class SessionManager {
 private:
  // fd: Session
  std::unordered_map<int, Session> pool;

 public:
  SessionManager() = default;
  bool attach(Session s);
  bool merge(std::vector<int> fd);
  bool detach(int fd);
  std::vector<int> getDest(int fd);
  ~SessionManager();
};
bool SessionManager::attach(Session s) {
  auto fdMap = s.getFd();
  for (auto&& [_, fd] : fdMap) {
    if (pool.count(fd)) return false;
  }
  for (auto&& [_, fd] : fdMap) {
    this->pool[fd] = s;
  }
  return true;
}
bool SessionManager::merge(std::vector<int> fd) {
  for (int i : fd) {
    if (this->pool.count(i) == 0) return false;
  }
  Session res;
  for (int i : fd) {
    if (!res.merge(this->pool[i])) return false;
  }
  for (int i : fd) {
    this->pool[i] = res;
  }
  return true;
}
bool SessionManager::detach(int fd) {
  if (this->pool.count(fd) == 0) return false;
  auto fdMap = this->pool[fd].getFd();
  for (auto&& [_, i] : fdMap) {
    if (this->pool.count(i) == 0) return false;
  }
  for (auto&& [_, i] : fdMap) {
    this->pool.erase(i);
  }
  return true;
}
std::vector<int> SessionManager::getDest(int fd) {
  if (this->pool.count(fd) == 0) return {};
  auto fdMap = this->pool[fd].getFd();
  std::vector<int> res;
  for (auto&& [_, i] : fdMap) {
    if (i != fd) res.emplace_back(i);
  }
  return res;
}
SessionManager::SessionManager(/* args */) {}

SessionManager::~SessionManager() {}
