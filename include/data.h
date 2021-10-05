#ifndef _DATA_H
#define _DATA_H 1
#include <ctime>
#include <sstream>
#include <string>
enum DataType {
  data_invalid,
  session_init,
  session_pair,
  delivery_data,
  conn_close,
};
struct DataHeader {
  DataType type;
  std::string timestamp;
};

struct DataBody {
  std::string content;
};

class Data {
 private:
  DataHeader header;
  DataBody body;
  static const DataFactory factory;
  static const int typeIdx = 0;
  static const int timestampIdx = 1;
  static const int contentIdx = timestampIdx + sizeof(time_t);

 public:
  Data() = default;
  Data(Data& other) = default;
  Data(DataType type);
  Data(DataType type, std::string content);
  Data(DataType type, std::string timestamp, std::string content);
  /// Layout: type(1 char) timestamp(time_t) content
  Data(std::string source);
  std::string to() const;
  long long getSize() const;
  bool isNull() const;
  DataHeader getHeader();
  DataBody getBody();
};
class DataFactory {
 private:
 public:
  DataFactory();
  ~DataFactory();
  static DataType charToType(char c) { return (DataType)c; }
  static char typeToChar(DataType t) { return (char)t; }
  template <class T>
  static T stringTo(std::string s) {
    T res;
    std::istringstream is(s);
    is >> res;
    return res;
  }
  template <class T>
  static std::string toString(T t) {
    std::ostringstream os;
    os << t;
    string res;
    std::istringstream is(os.str());
    is >> res;
    return res;
  }
};
#endif  // data.h