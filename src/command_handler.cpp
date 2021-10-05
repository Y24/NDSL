#include "command_handler.h"

#include <unistd.h>

#include <string>
CommandHandler::CommandHandler(/* args */) {}
Command CommandHandler::get(int fd) {
  char buf[100];
  int cnt = read(fd, buf, 100);
  if (cnt == -1 || cnt == 0) {
    return Command("");
  }
  /// Note: Once input more than 100 chars, the remaider is left to the next
  /// read, deal with the situation latter.
  buf[cnt] = '\0';
  return Command(std::string(buf));
}
CommandHandler::~CommandHandler() {}
