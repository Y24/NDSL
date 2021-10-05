#include "demo_data.h"

DemoData::DemoData(DataType type) { this->header.type = type; }
DemoData::DemoData(DataType type, std::string content) {
  this->header.type = type;
  this->body.content = content;
}
DemoData::DemoData(DataType type, std::string timestamp, std::string content) {
  this->header.type = type;
  this->header.timestamp = timestamp;
  this->body.content = content;
}
/// Layout: type(1 char) timestamp(time_t) content
DemoData::DemoData(std::string source) {
  this->header.type = factory.charToType(source[typeIdx]);
  this->header.timestamp = source.substr(timestampIdx, sizeof(time_t));
  this->body.content = source.substr(contentIdx);
}
std::string DemoData::to() const {
  return factory.typeToChar(header.type) + header.timestamp + body.content;
}
long long DemoData::getSize() const { return contentIdx + body.content.size(); }
bool DemoData::isNull() const { return header.type == data_invalid; }
DataHeader DemoData::getHeader() const { return header; }
DataBody DemoData::getBody() const { return body; }
DataFactory::DataFactory() {}

DataFactory::~DataFactory() {}