#include <ctime>
#include <sstream>
#include <string>
enum DataType {
  invalid,
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
  Data(DataType type) { this->header.type = invalid; }
  /// Layout: type(1 char) timestamp(long) content
  Data(std::string source) {
    this->header.type = factory.charToType(source[typeIdx]);
    /*     strftime(this->header.timestamp.data(), 64, "%Y-%m-%d %H:%M:%S",
                 source.substr(timestampIdx, sizeof(time_t)).data()); */
    this->header.timestamp = source.substr(timestampIdx, sizeof(time_t));
    this->body.content = source.substr(contentIdx);
  }
  Data(DataType type, std::string content) {
    this->header.type = type;
    this->body.content = content;
  }
  std::string to() const {
    return factory.typeToChar(header.type) + header.timestamp + body.content;
  }
  long long getSize() const { return contentIdx + body.content.size(); }
  bool isNull() const { return header.type == invalid; }
  DataHeader getHeader() const { return header; }
  DataBody getBody() const { return body; }
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

DataFactory::DataFactory() {}

DataFactory::~DataFactory() {}