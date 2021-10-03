struct DataHeader {
  int destination;
  char* timestamp;
};

struct DataBody {
  char* content;
};

struct Data {
  DataHeader header;
  DataBody body;
};
