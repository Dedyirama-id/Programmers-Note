#pragma once
#include <string>
#include "graph.h"
#include "binarytree.h"
#include "note.h"

#define MIN_PASSWORD_LENGTH 5
#define EMPTY 0

using namespace std;
using namespace gr;
using namespace tr;

struct Account {
private:
  string password;
public:
  unsigned int id;
  string username;

  Graph<string> *todos;
  BinaryTree<Note> *notes;

  Account() : id(EMPTY), username(""), password("") {}

  Account(unsigned int id, const string &username, const string &password, Graph<string> *todos, BinaryTree<Note> *notes)
    : id(id), username(username), password(hashing(password)), todos(todos), notes(notes)  {}

  ~Account() {}

  string hashing(const string &password) const {
    string hash;
    for (size_t i = 0; i < password.length(); i++) {
      hash += (password[i] * (i + 1)) % 126;
    }
    return hash;
  }

  bool validatePassword(const string &password) const {
    return this->password == hashing(password);
  }

  void serialize(ofstream &outFile) const {
    outFile.write(reinterpret_cast<const char *>(&id), sizeof(id));
    size_t usernameSize = username.length();
    outFile.write(reinterpret_cast<const char *>(&usernameSize), sizeof(usernameSize));
    outFile.write(username.c_str(), usernameSize);
    size_t passwordSize = password.length();
    outFile.write(reinterpret_cast<const char *>(&passwordSize), sizeof(passwordSize));
    outFile.write(password.c_str(), passwordSize);
  }

  void deserialize(ifstream &inFile) {
    inFile.read(reinterpret_cast<char *>(&id), sizeof(id));
    size_t usernameSize;
    inFile.read(reinterpret_cast<char *>(&usernameSize), sizeof(usernameSize));
    username.resize(usernameSize);
    inFile.read(&username[0], usernameSize);
    size_t passwordSize;
    inFile.read(reinterpret_cast<char *>(&passwordSize), sizeof(passwordSize));
    password.resize(passwordSize);
    inFile.read(&password[0], passwordSize);
  }
};