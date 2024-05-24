#include <iostream>
#include <string>
#include "src/class/hashtable.h"
#include "src/data-type/account.h"
#include "src/app/app.h"
#include "src/functions/utils.h"

using namespace std;

void printHelp();
unsigned int usernameToId(const string &username);
Account *registerAccount(ht::HashTable<Account> &accounts);
Account *login(ht::HashTable<Account> &accounts);

ht::HashTable<Account> *accounts = new ht::HashTable<Account>();

int main() {
  if (!accounts->loadFromBin("data/accounts.bin") || accounts->isEmpty()) {
    app::printWarning("No accounts found. Creating new accounts!");
    registerAccount(*accounts);
  }

  Account *loggedInAccount = login(*accounts);
  if (loggedInAccount != nullptr) {
    cout << "Login successful!" << endl;
    // Lakukan sesuatu setelah login berhasil
  }

  accounts->saveToBin("data/accounts.bin");
  accounts->destroyAll();
  delete accounts;
  return 0;
}

void printHelp() {
  app::printH1("# Help");
  cout << "No help yet :(" << endl;
}

Account *registerAccount(ht::HashTable<Account> &accounts) {
  string username = u::getStringInput("Username: ");
  unsigned int id = usernameToId(username);

  if (accounts.getRecord(id) != nullptr) {
    app::printError("Username already exists!");
    return nullptr;
  }

  string password = u::getStringInput("Password: ");

  Account newAccount(id, username, password);
  accounts.addRecord(newAccount);
  return accounts.getRecord(id);
}

Account *login(ht::HashTable<Account> &accounts) {
  string username = u::getStringInput("Username: ");
  unsigned int id = usernameToId(username);

  Account *account = accounts.getRecord(id);
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
