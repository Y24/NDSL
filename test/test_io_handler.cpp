#include <vector>

#include "content_generator.h"
#include "io_handler.h"
#include "test_framework.h"
/* build task settings configuration:
"args":[
        "-g",
        "/home/y24/NDSL/test/test_io_handler.cpp",
        "-I",
        "/home/y24/NDSL/include",
        "-o",
        "${fileDirname}/${fileBasenameNoExtension}",
        "/home/y24/NDSL/src/content_generator.cpp",
        "/home/y24/NDSL/src/demo_data.cpp",
        "/home/y24/NDSL/src/io_handler.cpp",
        ], */
std::vector<DemoData> generateDemoDataset() {
  DataFactory factory;
  ContentGenerator contentGenerator;
  std::vector<DemoData> res;
  res.emplace_back(data_invalid);
  res.emplace_back(session_init, contentGenerator.generate(12));
  res.emplace_back(session_pair, factory.toString(time(nullptr)),
                   contentGenerator.generate(12));
  res.emplace_back();
  res.emplace_back(res[2]);
  return res;
}
void test_write(DemoData data) {
  int fd = fileno(fopen("demo_in.txt", "w+"));
  IOHandler ioHandler(fd);
  ioHandler.write(data);
  close(fd);
}
DemoData test_read() {
  int fd = fileno(fopen("demo_in.txt", "r"));
  IOHandler ioHandler(fd);
  DemoData data = ioHandler.read();
  close(fd);
  return data;
}
int main(int argc, char const *argv[]) {
  auto demoDataset = generateDemoDataset();
  for (auto data : demoDataset) {
    test_write(data);
    auto newData = test_read();
    DemoTest::assertEqual(data.getHeader().timestamp,
                          newData.getHeader().timestamp);
    DemoTest::assertEqual(data.getHeader().type, newData.getHeader().type);
    DemoTest::assertEqual(data.getBody().content, newData.getBody().content);
  }
  return 0;
}
