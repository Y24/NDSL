#ifndef _CLIENT_EVENT_HANDLER_H
#define _CLIENT_EVENT_HANDLER_H 1
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <unordered_map>

#include "command.h"
#include "content_generator.h"
#include "event_manager.h"
#include "io_handler.h"
#include "session_manager.h"
#include "session_productor.h"
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
  void do_read(int fd, Data &data);
  void do_write(int fd, Data &data);

 public:
  EventHandler(int epollFd);
  void handle(epoll_event *events, int num, Data &data);
  ~EventHandler();
};

#endif  // client_event_handler.h