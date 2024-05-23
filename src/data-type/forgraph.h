#pragma once
#include <fstream>

using namespace std;

struct ForGraph {
  string name;
  string type;
  string description;
  string example;

  bool serialize(ofstream &out) {
    size_t len;

    len = name.size();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(name.c_str(), len);

    len = type.size();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(type.c_str(), len);

    len = description.size();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(description.c_str(), len);

    len = example.size();
    out.write(reinterpret_cast<const char *>(&len), sizeof(len));
    out.write(example.c_str(), len);

    return true;
  }

  bool deserialize(ifstream &in) {
    size_t len;

    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    char *buffer = new char[len + 1];
    in.read(buffer, len);
    buffer[len] = '\0';
    name = buffer;
    delete[] buffer;

    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    buffer = new char[len + 1];
    in.read(buffer, len);
    buffer[len] = '\0';
    type = buffer;
    delete[] buffer;

    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    buffer = new char[len + 1];
    in.read(buffer, len);
    buffer[len] = '\0';
    description = buffer;
    delete[] buffer;

    in.read(reinterpret_cast<char *>(&len), sizeof(len));
    buffer = new char[len + 1];
    in.read(buffer, len);
    buffer[len] = '\0';
    example = buffer;
    delete[] buffer;

    return true;
  }
};
