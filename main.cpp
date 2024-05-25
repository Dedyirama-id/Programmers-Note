#include <iostream>
#include <string>
#include "src/hashtable.h"
#include "src/graph.h"
#include "src/binarytree.h"
#include "src/queue.h"
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
void sendNotebook(const string &notebook);
void manageImport();
void printTodos();
void printTodoDetails(const int id);
void removeNote(const string &note);
void undoNotebookDelete();
void removeTodo(const int id);

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
  "sn",
  "import",
  "todos",
  "dt",
  "rn",
  "un",
  "rt"
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

    case 10:
      if (menu.commandValue == "") {
        app::printWarning("User cannot be empty!");
        app::printWarning("Try \"send <user>\" to send email!");
        u::wait();
        continue;
      }

      sendNotebook(menu.commandValue);
      break;

    case 11:
      manageImport();
      break;

    case 12:
      printTodos();
      u::wait();
      break;

    case 13:
      if (menu.commandValue == "") {
        app::printWarning("Todo id cannot be empty!");
        app::printWarning("Try \"dt <id>\" to print todo details!");
        u::wait();
        continue;
      }

      try {
        printTodoDetails(stoi(menu.commandValue));
      } catch (...) {
        app::printWarning("Invalid todo id!");
        u::wait();
      }
      break;

    case 14:
      if (menu.commandValue == "") {
        app::printWarning("Todo id cannot be empty!");
        app::printWarning("Try \"rn <title>\" to remove note!");
        u::wait();
        continue;
      }

      removeNote(menu.commandValue);
      break;

    case 15:
      undoNotebookDelete();
      break;

    case 16:
      if (menu.commandValue == "") {
        app::printWarning("Todo id cannot be empty!");
        app::printWarning("Try \"dt <id>\" to print todo details!");
        u::wait();
        continue;
      }

      try {
        removeTodo(stoi(menu.commandValue));
      } catch (...) {
        app::printWarning("Invalid todo id!");
        u::wait();
      }
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
  qu::Queue<Note> *notesQueue = new qu::Queue<Note>();
  st::Stack<Note> *notesStack = new st::Stack<Note>();
  Account newAccount(id, username, password);
  newAccount.todos = todos;
  newAccount.notes = notes;
  newAccount.notesQueue = notesQueue;
  newAccount.NotesStack = notesStack;

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

  gr::Vertex<string> *vertex = activeAccount->todos->getVerticesHead();
  while (vertex != nullptr) {
    if (vertex->data == todo) {
      app::printError("Todo already exists!");
      u::wait();
      return;
    }
    vertex = vertex->next;
  }

  bool success = activeAccount->todos->addVertex(todoId, todo);
  if (!success) {
    app::printError("Failed to add todo!");
    u::wait();
    return;
  }

  gr::Vertex<string> *vertexPrint = activeAccount->todos->getVerticesHead();
  if (activeAccount->todos->vertexCount <= 1) {
    app::printSuccess("Todo added!");
    u::wait();
    return;
  }

  while (true) {
    system("cls");
    app::printH1("# Todos Relationship: " + todo);
    app::printH2("ID Todo");
    while (vertexPrint != nullptr) {
      if (vertexPrint->id != todoId && vertexPrint->searchEdgeById(todoId) == false) cout << vertexPrint->id << ". " << vertexPrint->data << endl;

      vertexPrint = vertexPrint->next;
    }

    int todoIdInput = u::getIntInput("Todo id that depends on this: (0 to skip) ");
    if (todoIdInput == 0) break;

    if (activeAccount->todos->searchById(todoIdInput) == nullptr) {
      app::printError("Invalid todo id!");
      u::wait();
      continue;
    } else {
      activeAccount->todos->addEdgeById(todoIdInput, todoId);
      continue;
    }
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

void sendNotebook(const string &notebook) {
  tr::Node<Note> *bookNode = activeAccount->notes->search(notebook);
  if (bookNode == nullptr) {
    app::printError("Notebook not found!");
    u::wait();
    return;
  }

  string username = u::getStringInput("Send to user: ");
  unsigned int id = usernameToId(username);

  Account *targetAccount = accounts->getRecord(id);
  if (targetAccount == nullptr) {
    app::printError("User not found!");
    u::wait();
    return;
  }

  targetAccount->notesQueue->enqueue(bookNode->data);
  app::printSuccess("Note sent!");
  u::wait();
}

void manageImport() {
  app::Menu importMenu({ "Exit", "Accept", "Reject" });
  while (activeAccount->notesQueue->isEmpty() == false) {
    Note currentNote = activeAccount->notesQueue->peek();
    cout << currentNote.title << endl;

    for (int i = 0; i < currentNote.count; ++i) {
      cout << "- " << currentNote.content[i] << endl;
    }

    int choice = importMenu.getChoice();
    switch (choice) {
    case 0:
      return;
    case 1:
      activeAccount->notes->insert(currentNote.title, currentNote);
      activeAccount->notesQueue->dequeue();
      app::printSuccess("Note accepted!");
      break;
    case 2:
      activeAccount->notesQueue->dequeue();
      app::printError("Note rejected!");
      break;
    default:
      break;
    }
  }

  app::printWarning("Queue is empty!");
  u::wait();
}

void printTodos() {
  app::printH1("Todos");

  gr::Vertex<string> *vertexPrint = activeAccount->todos->getVerticesHead();
  while (vertexPrint != nullptr) {
    cout << vertexPrint->id << ". " << vertexPrint->data << endl;
    vertexPrint = vertexPrint->next;
  }
}

void printTodoDetails(const int id) {
  gr::Vertex<string> *current = activeAccount->todos->searchById(id);
  if (current == nullptr) {
    app::printError("Invalid todo id!");
    u::wait();
    return;
  }

  app::printH2("Todo Details");
  cout << "- Todo Id: " << current->id << endl;
  cout << "- Todo: " << current->data << endl;
  cout << "- Depends on: " << endl;
  gr::Edge<string> *edge = current->edgeList;
  while (edge != nullptr) {
    cout << "\t- [" << edge->vertexRef->id << "] " << edge->vertexRef->data << endl;
    edge = edge->next;
  }

  u::wait("\nEnter to continue...");
}

void removeNote(const string &title) {
  tr::Node<Note> *current = activeAccount->notes->search(title);
  if (current == nullptr) {
    app::printError("Invalid todo id!");
    u::wait();
    return;
  }

  Note toStack = current->data;
  activeAccount->NotesStack->push(toStack);
  activeAccount->notes->deleteNode(title);
  app::printSuccess("Note removed!");
  u::wait();
}

void undoNotebookDelete() {
  if (activeAccount->NotesStack->isEmpty()) {
    app::printError("Stack is empty!");
    u::wait();
    return;
  }

  Note currentNote = activeAccount->NotesStack->peek();
  activeAccount->notes->insert(currentNote.title, currentNote);
  activeAccount->NotesStack->pop();
  app::printSuccess("Note restored!");
  u::wait();
}

void removeTodo(const int id) {
  bool success = activeAccount->todos->deleteVertexById(id);
  if (!success) {
    app::printError("Todo not found!");
    u::wait();
    return;
  }
  app::printSuccess("Todo removed!");
  u::wait();
}