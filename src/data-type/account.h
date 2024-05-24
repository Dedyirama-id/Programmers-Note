#pragma once
#include <string>
#define MIN_PASSWORD_LENGTH 5
#define EMPTY 0

using namespace std;

struct Account {
private:
public:
  string password;
  unsigned int id;
  string username;

  Account() : username(""), password(""), id(EMPTY) {}
  Account(unsigned int id, string username, string password) : username(username), id(id) {
    this->password = hashing(password);
  }

  string hashing(const string &password) {
    string hash;
    for (int i = 0; i < password.length(); i++) {
      hash += password[i] * (i + 1) % 126;
    }

    return hash;
  }

  bool validatePassword(const string &password) {
    return this->password == hashing(password);
  }

  void serialize(ofstream &outFile) {
    outFile.write(reinterpret_cast<const char *>(&id), sizeof(id));
    size_t usernameSize = username.length();
    outFile.write(reinterpret_cast<const char *>(&usernameSize), sizeof(usernameSize));
    outFile.write(username.c_str(), usernameSize);
    size_t passwordSize = password.length();
    outFile.write(reinterpret_cast<const char *>(&passwordSize), sizeof(passwordSize));
    outFile.write(password.c_str(), passwordSize);
  }

  void deserialize(ifstream &file) {
    file.read(reinterpret_cast<char *>(&id), sizeof(id));
    size_t usernameSize;
    file.read(reinterpret_cast<char *>(&usernameSize), sizeof(usernameSize));
    username.resize(usernameSize);
    file.read(&username[0], usernameSize);
    size_t passwordSize;
    file.read(reinterpret_cast<char *>(&passwordSize), sizeof(passwordSize));
    password.resize(passwordSize);
    file.read(&password[0], passwordSize);
  }
};