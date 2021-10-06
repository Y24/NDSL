#include "command.h"
Command ::Command(std::string target) {
  StringSpliter spliter;
  DataFactory factory;
  auto res = spliter.split(target);
  if (res[0] == "start" && res.size() == 2) {
    type = session_start;
    para = factory.stringTo<int>(res[1]);
  } else if (res[0] == "test" && res.size() == 2) {
    type = session_test;
    para = factory.stringTo<int>(res[1]);
  } else if (res[1] == "stop" && res.size() == 1) {
    type = session_stop;
    para = 0;
  } else {
    type = command_invalid;
  }
}