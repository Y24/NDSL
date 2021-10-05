#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <unordered_map>

#include "content_generator.cpp"
#include "event_manager.cpp"
#include "io_handler.cpp"
#include "session_manager.cpp"
#include "session_productor.cpp"
#include "command.cpp"
class EventHandler {
 private:
  int epollFd;
  EventManager eventManager;
  SessionManager sessionManager;
  SessionProductor sessionProductor;
  DataFactory factory;
  /// clientfd: serverfd
  std::unordered_map<int, int> remotePool;
  /// clientfd: clientfd in the same session
  std::unordered_map<int, int> localPool;
  /// whether paired
  std::unordered_map<int, bool> flag;
  void do_read(int fd, Data &data) {
    if (fd == STDIN_FILENO) {
      /// TODO: command execute
      
      return;
    }
    IOHandler ioHandler(fd);
    auto data = ioHandler.read();
    std::vector<int> destination;
    switch (data.getHeader().type) {
      case data_invalid:
        eventManager.delete_event(fd, EPOLLIN);
        sessionManager.detach(fd);
        break;
      case conn_close:
        if (!sessionManager.detach(fd)) {
          fprintf(stderr, "ClientEventHandler do_read: conn_close fails\n");
        }
        break;
      case session_init:
        /// session_init at client side serves as a trigger event of
        /// session_pair
        int remoteFd = factory.stringTo<int>(data.getBody().content);
        if (remotePool.count(fd) == 0) {
          fprintf(stderr,
                  "ClientEventHandler do_read: session_init remotePoll check "
                  "fails\n");
        }
        remotePool[fd] = remoteFd;
        flag[fd] = true;
        if (localPool.count(fd) == 0) {
          fprintf(stderr,
                  "ClientEventHandler do_read: session_init localPool check "
                  "fails\n");
        } else if (flag[localPool[fd]]) {
          sessionManager.merge({fd, localPool[fd]});
        } else {
          /// do session_pair
          data =
              Data(session_pair, factory.toString(remotePool[localPool[fd]]));
          eventManager.modify_event(fd, EPOLLOUT);
        }
        break;
      case session_pair:
        /// session_pair at client side serves as a trigger event of
        /// delivery_data
        // check if pair is done, then do delivery_data
        bool status = data.getBody().content == "OK";
        if (status && flag[fd] && localPool.count(fd) && flag[localPool[fd]]) {
          data = Data(delivery_data, factory.toString(time(nullptr)),
                      ContentGenerator().generate(12));
          eventManager.modify_event(fd, EPOLLOUT);
        } else {
          fprintf(stderr, "ClientEventHandler do_read: session_pair fails\n");
        }
        break;
      case delivery_data:
        eventManager.add_event(STDOUT_FILENO, EPOLLOUT);
        break;
      default:
        fprintf(stderr, "ClientEventHandler do_read reach default case!\n");
        break;
    }
  }
  void do_write(int fd, Data &data) {
    if (fd == STDOUT_FILENO) {
      time_t cur = time(nullptr);
      tm *begin = new tm;
      strftime(data.getHeader().timestamp.data(), 64, "%Y-%m-%d %H:%M:%S",
               begin);
      fprintf(stdout, "Time delay: %lf ms\n",
              (double)cur - mktime(begin) / CLOCKS_PER_SEC * 1000);
      fprintf(stdout, "Data content: \n");
      delete begin;
    }
    IOHandler ioHandler(fd);
    if (!ioHandler.write(data)) {
      eventManager.delete_event(fd, EPOLLOUT);
    }
    // clean work
    data = Data();
  }

 public:
  EventHandler(int epollFd);
  void handle(epoll_event *events, int num, Data &data);
  ~EventHandler();
};
void EventHandler::handle(epoll_event *events, int num, Data &data) {
  for (int i = 0; i < num; i++) {
    int fd = events[i].data.fd;
    if (events[i].events & EPOLLIN)
      do_read(fd, data);
    else if (events[i].events & EPOLLOUT)
      do_write(fd, data);
  }
}
EventHandler::~EventHandler() {}
