#include <iostream>
#include "src/app/app.h"

using namespace std;

void printHelp();

string programRootPath;

int main(int argc, char const *argv[])
{
  programRootPath = argv[0];
  programRootPath = programRootPath.substr(0, programRootPath.find_last_of("\\/") + 1);

  // When program doesn't run with arguements
  if (argc == 1) {
    app::printWarning("No arguements included!");
    return 0;
  }

  // Creating arg object
  app::SingleArg arg(argc, argv);

  // Check arguements & flags
  if (arg.key == "help") {
    printHelp();
  } else {
    app::printError("Unknown command: " + arg.key);
    app::printWarning("Type 'help' for help!");
  }

  return 0;
}

void printHelp() {
  app::printH1("# Help");
  cout << "No help yet :(" << endl;
}
