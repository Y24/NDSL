#ifndef _COMMAND_H
#define _COMMAND_H 1
#include <string>

#include "demo_data.h"
#include "string_spliter.h"
enum CommandType {
  command_invalid,
  session_start,
  session_stop,
};
struct Command {
  CommandType type;
  int nSession;
  int nContent;

 public:
  Command(std::string target);
};
#endif  // command.h