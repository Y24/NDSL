/// Note: IOHandler wrap all the raw read and write and provide a higher-level
/// IO-fn
/// TODO:
#include <unistd.h>

#include "data.cpp"
class IOHandler {
 private:
  int fd;
  static const DataFactory factory;
  static void inPanic(int fd) {
    perror("read error:");
    close(fd);
  }
  static void outPanic(int fd) {
    perror("write error:");
    close(fd);
  }

 public:
  IOHandler(int fd);
  Data read();
  bool write(Data data);
  ~IOHandler();
};

IOHandler::IOHandler(int fd) : fd(fd) {}
Data IOHandler::read() {
  char buf[sizeof(long long) + 1];
  if (int cnt = ::read(fd, buf, sizeof(long long)); cnt == -1) {
    inPanic(fd);
    return Data(invalid);
  } else if (cnt == 0) {
    fprintf(stderr, "server close.\n");
    close(fd);
    return Data(conn_close);
  } else {
    buf[cnt] = '\0';
    int size = factory.stringTo<long long>(std::string(buf));
    char* source = new char[size + 1];
    if (int cnt = ::read(fd, source, size); cnt < size) {
      inPanic(fd);
      return Data(invalid);
    } else {
      source[size] = '\0';
      auto res = Data(std::string(source));
      delete[] source;
      return res;
    }
  }
}
bool IOHandler::write(Data data) {
  if (int cnt = ::write(fd, factory.toString<long long>(data.getSize()).data(),
                        sizeof(long long));
      cnt < sizeof(long long)) {
    outPanic(fd);
    return false;
  }
  if (int cnt = ::write(fd, data.to().data(), data.getSize());
      cnt < data.getSize()) {
    outPanic(fd);
    return false;
  }
  return true;
}
IOHandler::~IOHandler() {}
