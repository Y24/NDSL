#include "content_generator.h"
#include "demo_data.h"
#include "test_framework.h"
int main(int argc, char const *argv[]) {
  time_t now = time(nullptr);
  DataFactory factory;
  ContentGenerator contentGenerator;
  DemoData data = DemoData(session_init, factory.toString(now),
                           contentGenerator.generate(12));
  std::string str = data.to();
  DemoData newData = DemoData(str);
  return 0;
}
