#ifndef _CONTENT_GENERATOR_H
#define _CONTENT_GENERATOR_H 1
#include <random>
#include <string>
class ContentGenerator {
 private:
  /* data */
 public:
  ContentGenerator(/* args */);
  static std::string generate(int size);
  ~ContentGenerator();
};
#endif // content_genterator.h