#ifndef _COMMAND_HANDLER_H
#define _COMMAND_HANDLER_H 1
#include "command.h"
class CommandHandler {
 private:
  
 public:
  CommandHandler(/* args */);
  Command get(int fd);
  ~CommandHandler();
};
#endif  // command_handler.h