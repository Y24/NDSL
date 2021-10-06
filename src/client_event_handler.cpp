#include "client_event_handler.h"

#include <arpa/inet.h>
void EventHandler::do_clean() {
  // clean work
  for (auto [fd, _] : localPool) {
    close(fd);
  }
  remotePool.clear();
  localPool.clear();
  flag.clear();
}
void EventHandler::do_read(int fd, DemoData &data) {
  if (fd == STDIN_FILENO) {
    auto command = commandHandler.get(fd);
    std::vector<Session> sessions;
    switch (command.type) {
      case command_invalid:
        fprintf(stderr, "Input command is invalid!\n");
        break;
      case session_start:
        do_clean();
        sessions = sessionProductor.produce(2 * command.para);
        for (int i = 0; i < 2 * command.para; i += 2) {
          int fd1 = sessions[i].getFd().begin()->first;
          int fd2 = sessions[i + 1].getFd().begin()->first;
          localPool[fd1] = fd2;
          localPool[fd2] = fd1;
          flag[fd1] = flag[fd2] = false;
        }
        break;
      case session_test:
        break;
      case session_stop:
        do_clean();
        break;
      default:
        break;
    }
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
            DemoData(session_pair, factory.toString(remotePool[localPool[fd]]));
        eventManager.modify_event(fd, EPOLLOUT);
      }
      break;
    case session_pair:
      /// session_pair at client side serves as a trigger event of
      /// delivery_data
      // check if pair is done, then do delivery_data
      bool status = data.getBody().content == "OK";
      if (status && flag[fd] && localPool.count(fd) && flag[localPool[fd]]) {
        data = DemoData(delivery_data, factory.toString(time(nullptr)),
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
void EventHandler ::do_write(int fd, DemoData &data) {
  if (fd == STDOUT_FILENO) {
    time_t cur = time(nullptr);
    tm *begin = new tm;
    strftime(data.getHeader().timestamp.data(), 64, "%Y-%m-%d %H:%M:%S", begin);
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
  data = DemoData();
}
void EventHandler::handle(epoll_event *events, int num, DemoData &data) {
  for (int i = 0; i < num; i++) {
    int fd = events[i].data.fd;
    if (events[i].events & EPOLLIN)
      do_read(fd, data);
    else if (events[i].events & EPOLLOUT)
      do_write(fd, data);
  }
}
EventHandler::~EventHandler() {}
