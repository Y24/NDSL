#include "client_epoll_manager.h"
EpollManager::EpollManager(int size, int nEvents)
    : size(size),
      epollFd(epoll_create(size)),
      nEvents(nEvents),
      events(new epoll_event[nEvents]),
      handler(EventHandler(epollFd)) {}
EpollManager::EpollManager(EpollManager& other)
    : size(other.size),
      epollFd(other.epollFd),
      nEvents(other.nEvents),
      events(new epoll_event[nEvents]),
      data(other.data),
      handler(other.handler) {}
void EpollManager::work() {
  data = DemoData();
  while (true) {
    int ret = epoll_wait(epollFd, events, nEvents, -1);
    handler.handle(events, ret, data);
  }
}
EpollManager::~EpollManager() {
  delete[] events;
  close(epollFd);
}
