#include <sys/epoll.h>
#include <unistd.h>

#include "server_event_handler.cpp"
class EpollManager {
 private:
  int listenFd;
  int size;
  int epollFd;
  epoll_event* events;
  int nEvents;
  Data data;

  EventHandler handler;

 public:
  EpollManager(int listenFd, int size, Data data);
  EpollManager(EpollManager& other);
  void work();
  ~EpollManager();
};
EpollManager::EpollManager(EpollManager& other)
    : listenFd(other.listenFd),
      size(other.size),
      epollFd(other.epollFd),
      nEvents(other.nEvents),
      events(new epoll_event[nEvents]),
      data(Data()),
      handler(other.handler) {}
EpollManager::EpollManager(int listenFd, int size, Data data)
    : listenFd(listenFd),
      size(size),
      events(new epoll_event[nEvents]),
      nEvents(nEvents),
      epollFd(epoll_create(size)),
      data(data),
      handler(EventHandler(epollFd, listenFd)) {}
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
