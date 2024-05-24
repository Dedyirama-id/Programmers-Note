#include <iostream>
#include <string>
#include "src/hashtable.h"
#include "src/data-type/account.h"
#include "src/app.h"
#include "src/utils.h"

using namespace std;

void printHelp();
unsigned int usernameToId(const string &username);
Account *registerAccount();
Account *login();

ht::HashTable<Account> *accounts = new ht::HashTable<Account>();

int main() {
  system("cls");
  app::printH1("# Create First Account");
  registerAccount();

  system("cls");
  app::printH1("# Login");
  Account *account = login();

  if (account == nullptr) {
    app::printError("Login failed!");
    return 0;
  }

  while (true) {
    system("cls");
    app::printH1("# Programmers Notes - " + u::toUppercase(account->username));
    string command = u::getStringInput();

    app::SingleArg arg(command);

    if (arg.key == "help") {
      printHelp();
      u::wait();
    } else if (arg.key == "exit") {
      break;
    } else if (arg.key == "add") {
      /* code */
    } else if (arg.key == "view") {
      /* code */
    } else if (arg.key == "edit") {
      /* code */
    } else if (arg.key == "delete") {

    } else {
      app::printError("Invalid command!");
      u::wait();
    }

  }

  return 0;
}

void printHelp() {
  app::printH2("# Help");
  cout << "No help yet :(" << endl;
}

Account *registerAccount() {
  string username = u::getStringInput("Username: ");
  unsigned int id = usernameToId(username);

  if (accounts->getRecord(id) != nullptr) {
    app::printError("Username already exists!");
    return nullptr;
  }

  string password = u::getStringInput("Password: ");

  Account newAccount(id, username, password);
  accounts->addRecord(newAccount);
  return accounts->getRecord(id);
}

Account *login() {
  string username = u::getStringInput("Username: ");
  unsigned int id = usernameToId(username);

  Account *account = accounts->getRecord(id);
  if (account == nullptr) {
    app::printError("Account not found!");
    return nullptr;
  }

  string password = u::getStringInput("Password: ");
  if (!account->validatePassword(password)) {
    app::printError("Invalid password!");
    return nullptr;
  }
  return account;
}

unsigned int usernameToId(const string &username) {
  unsigned int id = 0;
  for (int i = 0; i < username.size(); ++i) {
    id += (username[i] - 64) * u::pow(10, username.size() - i - 1);
  }
  return id;
}
