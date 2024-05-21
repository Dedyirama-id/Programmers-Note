#pragma once 
#include <iostream>
#include <limits>
#include <string>
#include <ctime>
#include "color_styles.h"

using namespace std;

namespace u {
  unsigned long long pow(int base, int exponent) {
    unsigned long long result = 1;
    for (int i = 0; i < exponent; ++i) {
      result *= base;
    }
    return result;
  }

  bool validateDate(const string &date) {
    if (date.length() != 10) return false;

    char delimiter = date[2];

    if (delimiter != '/'
      && delimiter != '-'
      && delimiter != '.'
      && delimiter != ' '
      && delimiter != ',') {
      return false;
    }

    for (int i = 0; i < date.length(); ++i) {
      if (i == 2 || i == 5) {
        if (date[i] != delimiter) return false;
      } else {
        if (date[i] < '0' || date[i] > '9') return false;
      }
    }

    int year = stoi(date.substr(6, 4));
    int month = stoi(date.substr(3, 2));
    int day = stoi(date.substr(0, 2));

    if (year < 0) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
    if (month == 2) {
      bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
      if ((isLeapYear && day > 29) || (!isLeapYear && day > 28)) return false;
    }

    return true;
  }

  void clearBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }

  unsigned long long generateIdByTime(unsigned long long digits = 10) {
    
    return time(nullptr) % pow(10, digits + 1);
  }

  void wait(string text = "") {
    cout << text;
    cin.get();
  }

  string fixDoubleSpaces(string str) {
    while (str.find("  ") != string::npos) {
      str = str.replace(str.find("  "), 2, " ");
    }
    return str;
  }

  string removeSpaces(string str) {
    while (str.find(" ") != string::npos) {
      str = str.replace(str.find(" "), 1, "");
    }
    return str;
  }

  string capitalize(string str) {
    for (int i = 0; i < str.length(); i++) {
      if (tolower(str[i]) < 'a' || tolower(str[i]) > 'z') continue;
      else if (i == 0 || str[i - 1] == ' ') str[i] = toupper(str[i]);
      else str[i] = tolower(str[i]);
    }
    return str;
  }

  string toUppercase(string str) {
    for (int i = 0; i < str.length(); i++) {
      if (toupper(str[i]) < 'A' || toupper(str[i]) > 'Z') continue;
      str[i] = toupper(str[i]);
    };
    return str;
  }

  string toLowercase(string str) {
    for (int i = 0; i < str.length(); i++) {
      if (tolower(str[i]) < 'a' || tolower(str[i]) > 'z') continue;
      str[i] = tolower(str[i]);
    };
    return str;
  }

  int getIntInput(string text = "> ") {
    int input;
    while (true) {
      cin.clear();
      cout << text;
      cin >> input;
      if (cin.fail()) {
        cout << YELLOW << "Input must be a number!" << RESET << endl;
        cin.clear();
        clearBuffer();
      } else {
        clearBuffer();
        return input;
      }
    };
  }

  long getLongInput(string text = "> ") {
    long input;
    while (true) {
      cin.clear();
      cout << text;
      cin >> input;
      if (cin.fail()) {
        cout << YELLOW << "Input must be a number!" << RESET << endl;
        cin.clear();
        clearBuffer();
      } else {
        clearBuffer();
        return input;
      }
    };
  }

  unsigned long getUnsignedLongInput(string text = "> ") {
    unsigned long input;
    while (true) {
      cin.clear();
      cout << text;
      cin >> input;
      if (cin.fail()) {
        cout << YELLOW << "Input must be a number!" << RESET << endl;
        cin.clear();
        clearBuffer();
      } else {
        clearBuffer();
        return input;
      }
    };
  }

  unsigned long long getUnsignedLongLongInput(string text = "> ") {
    unsigned long long input;
    while (true) {
      cin.clear();
      cout << text;
      cin >> input;
      if (cin.fail()) {
        cout << YELLOW << "Input must be a number!" << RESET << endl;
        cin.clear();
        clearBuffer();
      } else {
        clearBuffer();
        return input;
      }
    };
  }

  string getStringInput(string text = "> ", int maxLength = 0) {
    string input;
    cout << text;
    getline(cin, input, '\n');
    if (maxLength > 0) input = input.substr(0, maxLength);
    return input;
  }

  bool getBoolInput(string text = "") {
    while (true) {
      string input = getStringInput(text + " (Y/N): ");
      if (input == "y" || input == "Y") return true;
      else if (input == "n" || input == "N") return false;
      cout << YELLOW << "   Invalid input!" << RESET;
    }
  }

  char getCharInput(string text = "> ") {
    string input = getStringInput(text);
    if (input.length() != 1) {
      cout << YELLOW << "Input must be a single character" << RESET << endl;
      return getCharInput(text);
    }
    return input[0];
  }

  string getDateInput(string text = "> ") {
    string input = getStringInput(text);
    if (!validateDate(input)) {
      cout << YELLOW << "Invalid input!" << RESET << endl;
      return getDateInput(text);
    }
    return input;
  }

  string getGenderInput(string text = "> ") {
    string input = getStringInput(text);
    input = toLowercase(input);
    if (input != "male" && input != "m" && input != "female" && input != "f") {
      cout << YELLOW << "Invalid input!" << RESET << endl;
      return getGenderInput(text);
    }

    if (input == "m") input = "male";
    else if (input == "f") input = "female";

    return input;
  }

  int getChoice(int min = 0, int max = 9, string text = "> ") {
    while (true) {
      int choice = getIntInput(text);
      if (choice < min || choice > max) cout << YELLOW << "Invalid choice!" << RESET << endl;
      else return choice;
    }
  }

  string cleanString(string str) {
    str = fixDoubleSpaces(str);
    for (int i = 0; i < str.length(); i++) {
      if (str[i] != ' ' && str[i] != '\t') break;
      str.erase(i, 1);
    }

    for (int i = str.length() - 1; i >= 0; i--) {
      if (str[i] != ' ' && str[i] != '\t') break;
      str.erase(i, 1);
    }

    return str;
  }

  void swap(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
  }

  void swap(string a, string b) {
    string temp = a;
    a = b;
    b = temp;
  }

  string getSubstring(string str, char delimiter = ' ', int nth = 1) {
    string result = "";
    for (size_t i = 0; i < nth; i++) {
      result = str.substr(0, str.find(delimiter));
    }

    return result;
  }
}