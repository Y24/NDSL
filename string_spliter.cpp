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

StringSpliter::StringSpliter(/* args */) {}
std::vector<std::string> StringSpliter::split(std::string text,
                                              std::string seperator = "\\s+") {
  std::regex ws_re(seperator);
  std::vector<std::string> v(
      std::sregex_token_iterator(text.begin(), text.end(), ws_re, -1),
      std::sregex_token_iterator());
  return v;
}
StringSpliter::~StringSpliter() {}
