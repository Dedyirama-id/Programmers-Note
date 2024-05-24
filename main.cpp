#include <iostream>
#include <string>
#include "src/hashtable.h"
#include "src/graph.h"
#include "src/binarytree.h"
#include "src/account.h"
#include "src/note.h"
#include "src/app.h"
#include "src/utils.h"

using namespace std;

void printHelp();
unsigned int usernameToId(const string &username);
Account *registerAccount();
Account *login();
void removeAccount();
void addTodo(const string &todo);
void addNotebook(const string &notebook);
void addNote(const string &note);
void openNotebook(const string &notebook);

ht::HashTable<Account> *accounts = new ht::HashTable<Account>();

Account *activeAccount = nullptr;

app::CliMenu menu({
  "exit",
  "help",
  "login",
  "logout",
  "au",
  "ru",
  "do",
  "cn",
  "an",
  "on",
  });

int main() {
  system("cls");
  app::printH1("# Create First Account");
  registerAccount();

  system("cls");
  app::printH1("# Login");
  activeAccount = login();

  if (activeAccount == nullptr) {
    app::printError("Login failed!");
    return 0;
  }

  while (true) {
    system("cls");
    app::printH1("# Programmers Notes - " + u::toUppercase(activeAccount->username));
    menu.getCommand();

    switch (menu.commandNum) {
    case 0:
      break;
    case 1:
      printHelp();
      u::wait();
      break;
    case 2:
      if (activeAccount != nullptr) {
        app::printWarning("Already logged in!");
        u::wait();
        continue;
      }

      activeAccount = login();
      break;
    case 3:
      activeAccount = nullptr;
      app::printSuccess("Logged out!");
      u::wait();

      while (activeAccount == nullptr) activeAccount = login();
      break;
    case 4:
      app::printH2("Create new user account");
      registerAccount();
      break;
    case 5:
      app::printH2("Remove user account");
      removeAccount();
      break;
    case 6:
      if (menu.commandValue == "") {
        app::printWarning("Todo cannot be empty!");
        app::printWarning("Try \"do <todo>\" to add a new todo!");
        u::wait();
        continue;
      }

      addTodo(menu.commandValue);
      break;
    case 7:
      if (menu.commandValue == "") {
        app::printWarning("Notebook name cannot be empty!");
        app::printWarning("Try \"cn <notebook>\" to add a new notebook!");
        u::wait();
        continue;
      }
      addNotebook(menu.commandValue);
      break;
    case 8:
      if (menu.commandValue == "") {
        app::printWarning("Note cannot be empty!");
        app::printWarning("Try \"an <note>\" to add note!");
        u::wait();
        continue;
      }
      addNote(menu.commandValue);
      break;
    case 9:
      if (menu.commandValue == "") {
        app::printWarning("Notebook name cannot be empty!");
        app::printWarning("Try \"on <notebook>\" to open note!");
        u::wait();
        continue;
      }

      openNotebook(menu.commandValue);
      break;
    default:
      app::printWarning("Invalid command!");
      u::wait();
      break;
    };

    if (menu.commandNum == 0) break;
    menu.commandNum = -1;
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

  gr::Graph<string> *todos = new gr::Graph<string>();
  tr::BinaryTree<Note> *notes = new tr::BinaryTree<Note>();
  Account newAccount(id, username, password, todos, notes);
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

void removeAccount() {
  string username = u::getStringInput("Username: ");
  unsigned int id = usernameToId(username);

  Account *account = accounts->getRecord(id);
  if (account == nullptr) {
    app::printError("Account not found!");
    return;
  }

  accounts->removeRecord(id);
  app::printSuccess("Account removed!");
  u::wait();
}

void addTodo(const string &todo) {
  unsigned int todoId = activeAccount->todos->vertexCount + 1;
  bool success = activeAccount->todos->addVertex(todoId, todo);

  if (!success) {
    app::printError("Failed to add todo!");
    u::wait();
    return;
  }
  app::printSuccess("Todo added!");
  u::wait();
}

void addNotebook(const string &notebook) {
  activeAccount->notes->insert(notebook, Note(notebook));
  app::printSuccess("Notebook added!");
  u::wait();
}

void addNote(const string &note) {
  app::printH2("Notebook available:");
  activeAccount->notes->inOrderTraversal();

  string notebook = u::getStringInput("Please select a notebook: ");

  if (activeAccount->notes->search(notebook) == nullptr) {
    app::printError("Notebook not found!");
    u::wait();
    return;
  }

  tr::Node<Note> *bookNode = activeAccount->notes->search(notebook);
  bookNode->data.addContent(note);
  app::printSuccess("Note added!");
  u::wait();
}

void openNotebook(const string &notebook) {
  tr::Node<Note> *bookNode = activeAccount->notes->search(notebook);
  if (bookNode == nullptr) {
    app::printError("Notebook not found!");
    u::wait();
    return;
  }

  app::printH2(bookNode->id);
  for (int i = 0; i < bookNode->data.count; ++i) {
    cout << "- " << bookNode->data.content[i] << endl;
  }
  u::wait();
}