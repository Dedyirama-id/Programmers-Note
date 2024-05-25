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

// Function Prototype
bool statusCheck(const bool mustLoggedIn = false, const bool mustHaveCommandValue = false);
void printHelp();
unsigned int usernameToId(const string &username);
Account *registerAccount();
Account *login();
void logout();
void removeAccount();
void addTodo(const string &todo);
void addNotebook(const string &notebook);
void addNote(const string &note);
void openNotebook(const string &notebook);
void sendNotebook(const string &notebook);
void manageImport();
void printTodos();
void printTodoDetails(const int id);
void deleteNotebook(const string &note);
void undoNotebookDelete();
void removeTodo(const int id);
void showWhatTodo();
void editNote(const string &title);
void editNote(tr::Node<Note> *target);
void editTodo(const int id);

// Global Object
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
  "rt",
  "wtd",
  "sid",
  "smp",
  "en", 
  "et"
  });

int main() {
  system("cls");
  app::printH2("You need to create an account first!");
  registerAccount();

  while (activeAccount == nullptr) {
    system("cls");
    app::printH1("# Login");
    activeAccount = login();
  }

  while (true) {
    system("cls");
    if (activeAccount == nullptr) app::printH1("# Programmers Notes: Not logged in");
    else app::printH1("# Programmers Notes: " + u::toUppercase(activeAccount->username));
    menu.getCommand();

    switch (menu.commandNum) {
    case 0: // exit
      break;

    case 1: // help
      printHelp();
      u::wait();
      break;

    case 2: // login
      if (activeAccount != nullptr) {
        app::printWarning("Already logged in!");
        u::wait();
        continue;
      }

      activeAccount = login();
      break;

    case 3: // logout
      if (activeAccount == nullptr) {
        app::printWarning("Not logged in!");
        u::wait();
        continue;
      }

      logout();
      break;

    case 4: // au - add user
      if (!statusCheck(false, false)) continue;
      app::printH2("Create new user account");
      registerAccount();
      break;

    case 5: // ru - remove user
      app::printH2("Remove user account");
      removeAccount();
      break;

    case 6: // do - add todo
      if (!statusCheck(true, true)) break;
      addTodo(menu.commandValue);
      break;

    case 7: // cn - create notebook
      if (!statusCheck(true, true)) continue;
      addNotebook(menu.commandValue);
      break;

    case 8: // an - add note
      if (!statusCheck(true, true)) continue;
      addNote(menu.commandValue);
      break;

    case 9: // on - open notebook
      if (!statusCheck(true, true)) continue;
      openNotebook(menu.commandValue);
      break;

    case 10: // sn - send notebook
      if (!statusCheck(true, true)) continue;
      sendNotebook(menu.commandValue);
      break;

    case 11: // import - manage notebook import
      if (!statusCheck(true, true)) continue;
      manageImport();
      break;

    case 12: // todos
      if (!statusCheck(true, false)) continue;
      printTodos();
      u::wait();
      break;

    case 13: // dt - print todo details
      if (!statusCheck(true, true)) continue;
      try {
        printTodoDetails(stoi(menu.commandValue));
        u::wait("\nEnter to continue...");
      }
      catch (...) {
        app::printWarning("Invalid todo id!");
        u::wait();
      }
      break;

    case 14: // rn - remove notebook
      if (!statusCheck(true, true)) continue;
      if (menu.commandValue == "") {
        app::printWarning("Todo id cannot be empty!");
        app::printWarning("Try \"rn <title>\" to remove note!");
        u::wait();
        continue;
      }

      deleteNotebook(menu.commandValue);
      break;

    case 15: // un - undo notebook delete
      if (!statusCheck(true, true)) continue;
      undoNotebookDelete();
      break;

    case 16: // rm - remove todo
      if (!statusCheck(true, true)) continue;
      try {
        removeTodo(stoi(menu.commandValue));
      }
      catch (...) {
        app::printWarning("Invalid todo id!");
        u::wait();
      }
      break;

    case 17: // wtd - show what to do
      if (!statusCheck(true, false)) continue;
      showWhatTodo();
      u::wait("Enter to continue...");
      break;

    case 18: // sid - sort todo list by id
      if (!statusCheck(true, true)) continue;
      app::printH2("Sort todo list by Id");
      activeAccount->todos->insertionSortById();
      printTodos();
      u::wait();
      break;

    case 19: // smp - sort todo list by most possible to do
      if (!statusCheck(true, true)) continue;
      app::printH2("Sort todo list by most possible");
      activeAccount->todos->insertionSortByDegreeAscending();
      printTodos();
      u::wait();
      break;

    case 20: // en - edit note
      if (!statusCheck(true, true)) continue;
      editNote(menu.commandValue);
      break;

    case 21: // et - edit todo dependencies
      if (!statusCheck(true, true)) continue;
      try {
        editTodo(stoi(menu.commandValue));
      }
      catch (...) {
        app::printWarning("Invalid todo id!");
        u::wait();
      }
      break;

    default:
      app::printWarning("Invalid command!");
      menu.reset();
      u::wait();
      break;
    };

    if (menu.commandNum == 0) break;
    menu.reset();
  }
  return 0;
}

bool statusCheck(const bool mustLoggedIn, const bool mustHaveCommandValue) {
  if (mustLoggedIn && activeAccount == nullptr) {
    app::printWarning("You must login first!");
    u::wait();
    return false;
  }

  if (mustHaveCommandValue && menu.commandValue == "") {
    app::printWarning("Command value cannot be empty!");
    app::printWarning("Try \"<command> <value>\"!");
    cout << "For more info: type \"help\"" << endl;
    u::wait();
    return false;
  }

  return true;
}

void printHelp() {
  app::printH2("# Help");
  cout << "Available Commands:" << endl;
  cout << "  exit          - Exit the application" << endl;
  cout << "  help          - Show this help message" << endl;
  cout << "  login         - Login to your account" << endl;
  cout << "  logout        - Logout of your account" << endl;
  cout << "  au            - Add a new user account" << endl;
  cout << "  ru            - Remove a user account" << endl;
  cout << "  do <todo>     - Add a new todo item" << endl;
  cout << "  cn <notebook> - Create a new notebook" << endl;
  cout << "  an <note>     - Add a new note to a notebook" << endl;
  cout << "  on <notebook> - Open a notebook to view its contents" << endl;
  cout << "  sn <notebook> - Send a notebook to another user" << endl;
  cout << "  import        - Manage notebook imports" << endl;
  cout << "  todos         - List all todos" << endl;
  cout << "  dt <id>       - Display details of a specific todo" << endl;
  cout << "  rn <notebook> - Remove a notebook" << endl;
  cout << "  un            - Undo the last notebook deletion" << endl;
  cout << "  rt <id>       - Remove a todo" << endl;
  cout << "  wtd           - Show what todos can be done now" << endl;
  cout << "  sid           - Sort todo list by ID" << endl;
  cout << "  smp           - Sort todo list by most possible to do" << endl;
  cout << endl;
  cout << "Note: Some commands require you to be logged in and/or provide additional values." << endl;
}

Account *registerAccount() {
  string username = u::getStringInput("Username: ");
  unsigned int id = usernameToId(username);

  if (accounts->getRecord(id) != nullptr) {
    app::printError("Username already exists!");
    u::wait();
    return nullptr;
  }

  string password, confirmPassword;
  do {
    password = u::getStringInput("Password: ");
    confirmPassword = u::getStringInput("Confirm password: ");
    if (password != confirmPassword) {
      app::printError("Passwords do not match!");
      u::wait();
    }
  } while (password != confirmPassword);

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
    u::wait();
    return nullptr;
  }

  string password = u::getStringInput("Password: ");
  if (!account->validatePassword(password)) {
    app::printError("Invalid password!");
    u::wait();
    return nullptr;
  }
  return account;
}

void logout() {
  if (u::getBoolInput("Are you sure?")) {
    activeAccount = nullptr;
    app::printSuccess("Logged out!");
  } else {
    app::printWarning("Cancelled!");
  }
  u::wait();
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
    u::wait();
    return;
  }

  string password = u::getStringInput("Enter account password to delete: ");
  if (!account->validatePassword(password)) {
    app::printError("Invalid password!");
    u::wait();
    return;
  }

  if (!u::getBoolInput("Are you sure?")) {
    app::printWarning("Cancelled!");
    u::wait();
    return;
  }

  if (activeAccount == account) {
    activeAccount = nullptr;
  }

  accounts->removeRecord(id);
  app::printSuccess("Account removed!");
  u::wait();
}

void addTodo(const string &todo) {
  const unsigned int todoId = activeAccount->todos->idCount + 1;
  gr::Vertex<string> *vertex = activeAccount->todos->getVerticesHead();
  while (vertex != nullptr) {
    if (vertex->data == todo) {
      app::printError("Todo already exists!");
      u::wait();
      return;
    } else if (vertex->id == todoId) {
      app::printError("Todo ID already exists!");
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

  activeAccount->todos->idCount++;
  app::printSuccess("Todo added!");
  u::wait();
  if (activeAccount->todos->vertexCount <= 1) return;

  while (true) {
    system("cls");
    app::printH1("# Todos Relationship: " + todo);
    app::printH2("ID Todo");
    gr::Vertex<string> *vertexPrint = activeAccount->todos->getVerticesHead();
    int printCount = 0;
    while (vertexPrint != nullptr) {
      if (vertexPrint->id != todoId && vertexPrint->searchEdgeById(todoId) == false) {
        cout << vertexPrint->id << ". " << vertexPrint->data << endl;
        printCount++;
      }

      vertexPrint = vertexPrint->next;
    }

    if (printCount == 0) {
      app::printWarning("No options available!");
      u::wait();
      return;
    }

    int todoIdInput = u::getIntInput("Todo id that depends on " + todo + ": (0 to skip) ");

    if (todoIdInput == 0) break;
    gr::Vertex<string> *targetVertex = activeAccount->todos->searchById(todoIdInput);
    if (targetVertex->searchEdgeById(todoId) != nullptr) {
      app::printWarning("Todo already connected!");
      u::wait();
    }

    if (targetVertex == nullptr) {
      app::printError("Invalid todo id!");
      u::wait();
    } else {
      activeAccount->todos->addEdgeById(todoIdInput, todoId);
    }
  }
}

void addNotebook(const string &notebook) {
  if (activeAccount->notes->search(notebook) != nullptr) {
    app::printError("Notebook already exists!");
    u::wait();
    return;
  }

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
    cout << "[" << i + 1 << "] " << bookNode->data.content[i] << endl;
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
      if (activeAccount->notes->search(currentNote.title) != nullptr) {
        app::printError("Note with same title already exists!");
        u::wait();
        continue;
      }

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
  app::printH2("ID  Todo");
  while (vertexPrint != nullptr) {
    cout << vertexPrint->id << ".  " << vertexPrint->data << endl;
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
  int printCount = 0;
  while (edge != nullptr) {
    cout << "\t- [" << edge->vertexRef->id << "] " << edge->vertexRef->data << endl;
    edge = edge->next;
    printCount++;
  }

  if (printCount == 0) app::printWarning("No dependencies!");
}

void deleteNotebook(const string &title) {
  tr::Node<Note> *current = activeAccount->notes->search(title);
  if (current == nullptr) {
    app::printError("Invalid note!");
    u::wait();
    return;
  }

  if (!u::getBoolInput("Are you sure you want to delete this note?")) {
    app::printWarning("Note deletion cancelled!");
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
  app::printSuccess("Note \"" + currentNote.title + "\" restored!");
  u::wait();
}

void removeTodo(const int id) {
  if (activeAccount->todos->searchById(id) == nullptr) {
    app::printError("Invalid todo id!");
    u::wait();
    return;
  }

  if (!u::getBoolInput("Are you sure you want to delete this todo?")) {
    app::printWarning("Todo deletion cancelled!");
    u::wait();
    return;
  }

  bool success = activeAccount->todos->deleteVertexById(id);
  if (!success) {
    app::printError("Failed to remove todo!");
    u::wait();
    return;
  }

  app::printSuccess("Todo removed!");
  u::wait();
}

void showWhatTodo() {
  if (activeAccount->todos->isEmpty()) {
    app::printSuccess("Nothing to do! Todo list is empty!");
    u::wait();
    return;
  }

  activeAccount->todos->insertionSortByDegreeAscending();
  gr::Vertex<string> *todo = activeAccount->todos->getVerticesHead();
  app::printWarning("List of todo that possible to be done now:");
  int count = 1;
  app::printH2("No  Todo");
  while (todo != nullptr) {
    if (todo->degree > 0) break;
    cout << count << ".  " << todo->data << endl;
    todo = todo->next;
    ++count;
  }
}

void editNote(const string &title) {
  tr::Node<Note> *target = activeAccount->notes->search(title);
  if (target == nullptr) {
    app::printError("Invalid notebook!");
    u::wait();
    return;
  }

  app::Menu editNotebookMenu({ "Exit", "Add Note", "Edit Note", "Delete Note" });
  while (true) {
    system("cls");
    app::printH1("Edit Notebook: " + title);
    for(int i = 0; i < target->data.count; ++i) {
      cout << "[" << i + 1 << "] " << target->data.content[i] << endl;
    }

    app::printDivider();
    string stringInput;
    int intInput;
    int choice = editNotebookMenu.getChoice();
    switch (choice) {
    case 0:
      return;
    case 1:
      stringInput = u::getStringInput("Enter new content: ");
      target->data.addContent(stringInput);
      break;
    case 2:
      editNote(target);
      break;
    case 3:
      intInput = u::getIntInput("Number of content to delete: ") - 1;
      if (intInput < 0 || intInput >= target->data.count) {
        app::printError("Invalid index!");
        u::wait();
        break;
      }

      if (!u::getBoolInput("Are you sure you want to delete this note?")) {
        app::printWarning("Note deletion cancelled!");
        u::wait();
        break;
      }

      target->data.deleteContent(intInput);
      app::printSuccess("Note removed!");
      u::wait();
      break;
    default:
      break;
    }
  }
}

void editNote(tr::Node<Note> *target) {
  int indexToEdit = u::getIntInput("Number of content to edit: ") - 1;
  if (indexToEdit < 0 || indexToEdit >= target->data.count) {
    app::printError("Invalid index!");
    u::wait();
    return;
  }

  string newContent = u::getStringInput("Enter new content: ");
  target->data.content[indexToEdit] = newContent;
  app::printSuccess("Content updated!");
  u::wait();
}

void editTodo(const int id) {
  gr::Vertex<string> *target = activeAccount->todos->searchById(id);
  if (target == nullptr) {
    app::printError("Todo not found!");
    u::wait();
    return;
  }

  app::Menu editTodoMenu({ "Exit", "Edit Todo", "Remove Todo Dependencies" });
  while (true) {
    system("cls");
    app::printH1("Edit Todo: " + target->data);
    printTodoDetails(id);
    app::printDivider();
    string stringInput;
    int intInput;
    int choice = editTodoMenu.getChoice();
    switch (choice) {
    case 0:
      return;
    case 1:
      stringInput = u::getStringInput("Enter new todo: ");
      target->data = stringInput;
      break;
    case 2:
      intInput = u::getIntInput("Id of todo dependencies to remove: ");
      if (target->searchEdgeById(intInput) == nullptr) {
        app::printError("Invalid id!");
        u::wait();
        break;
      }

      if (!u::getBoolInput("Are you sure you want to remove this dependency?")) {
        app::printWarning("Dependency removal cancelled!");
        u::wait();
        break;
      }

      target->deleteEdgeById(intInput);
      app::printSuccess("Dependency removed!");
      u::wait();
      break;
    default:
      break;
    }
  }
}

