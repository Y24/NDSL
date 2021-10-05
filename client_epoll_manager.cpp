#include <sys/epoll.h>
#include <unistd.h>

#include "client_event_handler.cpp"
class EpollManager {
 private:
  int size;
  int epollFd;
  epoll_event* events;
  int nEvents;
  Data data;

  EventHandler handler;

 public:
  EpollManager(int size, int nEvents);
  EpollManager(EpollManager& other);
  void work();
  ~EpollManager();
};
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
      data(Data()),
      handler(other.handler) {}
void EpollManager::work() {
  data = Data();
  while (true) {
    int ret = epoll_wait(epollFd, events, nEvents, -1);
    handler.handle(events, ret, data);
  }
}
EpollManager::~EpollManager() {
  delete[] events;
  close(epollFd);
}
