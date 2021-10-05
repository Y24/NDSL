#include "data.h"

Data::Data() = default;
Data::Data(Data& other) = default;
Data::Data(DataType type) { this->header.type = type; }
Data::Data(DataType type, std::string content) {
  this->header.type = type;
  this->body.content = content;
}
Data::Data(DataType type, std::string timestamp, std::string content) {
  this->header.type = type;
  this->header.timestamp = timestamp;
  this->body.content = content;
}
/// Layout: type(1 char) timestamp(time_t) content
Data::Data(std::string source) {
  this->header.type = factory.charToType(source[typeIdx]);
  this->header.timestamp = source.substr(timestampIdx, sizeof(time_t));
  this->body.content = source.substr(contentIdx);
}
std::string Data::to() const {
  return factory.typeToChar(header.type) + header.timestamp + body.content;
}
long long Data::getSize() const { return contentIdx + body.content.size(); }
bool Data::isNull() const { return header.type == data_invalid; }
DataHeader Data::getHeader() const { return header; }
DataBody Data::getBody() const { return body; }
DataFactory::DataFactory() {}

DataFactory::~DataFactory() {}