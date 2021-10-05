#ifndef _STRING_SPLITER_H
#define _STRING_SPLITER_H 1
#include <regex>
#include <string>
#include <vector>
class StringSpliter {
 private:
  /* data */
 public:
  StringSpliter(/* args */);
  std::vector<std::string> split(std::string text, std::string seperator);
  ~StringSpliter();
};
#endif // string_spliter.h
