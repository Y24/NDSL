#include <random>
#include <string>
class ContentGenerator {
 private:
  /* data */
 public:
  ContentGenerator(/* args */);
  static std::string generate(int size) {
    std::string res(size, '0');
    std::random_device rd;
    std::default_random_engine rand(rd());
    for (int i = 0; i < size; i++) {
      int cur = rand();
      switch (cur % 3) {
        case 1:
          res[i] = 'A' + rand() % 26;
          break;
        case 2:
          res[i] = 'a' + rand() % 26;
          break;
        default:
          res[i] = '0' + rand() % 10;
          break;
      }
    }
    return res;
  }
  ~ContentGenerator();
};

ContentGenerator::ContentGenerator(/* args */) {}
ContentGenerator::~ContentGenerator() {}
