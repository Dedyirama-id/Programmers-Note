#pragma once
#include <string>
#define MIN_PASSWORD_LENGTH 5
#define EMPTY 0

using namespace std;

struct Account {
private:
  string password;
public:
  unsigned int id;
  string username;

  Account() : username(""), password(""), id(EMPTY) {}
  Account(unsigned int id, string username, string password) : username(username), id(id) {
    this->password = hashing(password);
  }

  string hashing(const string &password) {
    string hash;
    for (int i = 0; i < password.length(); i++) {
      hash += password[i] * (i + 1) % 127;
    }

    return hash;
  }

  bool validatePassword(const string &password) {
    return this->password == hashing(password);
  }
};