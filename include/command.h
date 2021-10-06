#ifndef _COMMAND_H
#define _COMMAND_H 1
#include <string>

#include "demo_data.h"
#include "string_spliter.h"
enum CommandType {
  command_invalid,
  session_start,
  session_test,
  session_stop,
};
struct Command {
  CommandType type;
  int para;

 public:
  Command(std::string target);
};
#endif  // command.h