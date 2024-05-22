#include <iostream>

using namespace std;

namespace test {
  void printSuccess(const string &text) {
    cout << "\033[1;32m[SUCCESS] " << text << "\033[0m" << endl;
  }

  void printError(const string &text) {
    cerr << "\033[1;31m[ERROR] " << text << "\033[0m" << endl;
  }

  void printWarning(const string &text) {
    cout << "\033[1;33m[WARNING] " << text << "\033[0m" << endl;
  }
}
