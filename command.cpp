#include <string>

#include "data.cpp"
#include "string_spliter.cpp"
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
  Command(std::string target) {
    StringSpliter spliter;
    DataFactory factory;
    auto res = spliter.split(target);
    if (res[0] == "start" && res.size() == 3) {
      type = session_start;
      nSession = factory.stringTo<int>(res[1]);
      nContent = factory.stringTo<int>(res[2]);
    } else if (res[1] == "stop" && res.size() == 1) {
      type = session_stop;
      nSession = nContent = 0;
    } else {
      type = command_invalid;
    }
  }
};