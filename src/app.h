#pragma once
#include <iostream>
#include <conio.h>
#include "color_styles.h"
#include "utils.h"

using namespace std;

namespace app {
  class Menu {
  public:
    string *menuList;
    int menuSize;
    int currentOption = 1;

    Menu(initializer_list<string> menuItems) {
      menuSize = menuItems.size();
      menuList = new string[menuSize];
      int index = 0;
      for (auto &item : menuItems) {
        menuList[index++] = item;
      }
    }

    ~Menu() {
      delete[] menuList;
    }

    void printMenu() {
      cout << "\e[?25l";
      for (int i = 1; i < menuSize; i++) {
        if (i == currentOption) cout << BOLD << BLUE;
        cout << (currentOption == i ? "> " : "  ") << menuList[i] << endl;
        cout << RESET;
      }

      if (currentOption == 0) cout << BOLD << BLUE;
      cout << (currentOption == 0 ? "> " : "  ") << menuList[0] << endl;
      cout << RESET;
    }

    int getChoice() {
      cout << "\e[?25l";
      while (true) {
        printMenu();

        cout << "\033[" << menuSize << "A";

        char key = _getch();

        switch (key) {
        case 72: // Panah atas
          currentOption = (currentOption - 1 + menuSize) % menuSize;
          break;
        case 80: // Panah bawah
          currentOption = (currentOption + 1) % menuSize;
          break;
        case 13: // Tombol Enter
        case 32: // Tombol spasi
          cout << "\e[?25h";
          cleanMenu();
          return currentOption;
        }
      }
    }

    void cleanMenu() {
      for (int i = 1; i < menuSize; i++) {
        for (int j = 0; j < menuList[i].length() + 2; j++) cout << " ";
        cout << endl;
      }

      for (int i = 0; i < menuList[0].length() + 2; i++) cout << " ";
      cout << endl;
      cout << "\033[" << menuSize << "A";
    }
  };

  class SingleArg {
  public:
    string key = "";
    string value = "";

    SingleArg(int argc, char const *argv[]) {
      if (argc > 1) {
        key = argv[1];
        key = u::toLowercase(key);

        for (int i = 2; i < argc; i++) {
          value += (i > 2 ? " " : "") + string(argv[i]);
        }
        toupper(value[0]);
      }
    }

    SingleArg(string command) {
      if (command.length() > 0) {
        key = command.substr(0, command.find(" "));
        key = u::toLowercase(key);
        value = command.substr(command.find(" ") + 1);
      }
    }
  };

  class CliMenu {
  public:
    string *menuList;
    int menuSize;
    int commandNum = -1;
    string commandValue = "";


    CliMenu(initializer_list<string> menuItems) {
      menuSize = menuItems.size();
      menuList = new string[menuSize];
      int index = 0;
      for (const auto &item : menuItems) {
        menuList[index++] = item;
      }
    }

    ~CliMenu() {
      delete[] menuList;
    }

    void getCommand() {
      reset();
      string input = u::getStringInput();
      size_t spacePos = input.find(" ");
      string commandInput = input.substr(0, spacePos);

      if (spacePos == string::npos) {
        commandValue = "";
      } else {
        commandValue = input.substr(spacePos + 1);
      }

      for (int i = 0; i < menuSize; i++) {
        if (commandInput == menuList[i]) {
          commandNum = i;
          return;
        }
      }

      commandNum = -1;
    }

    void reset() {
      commandNum = -1;
      commandValue = "";
    }
  };

  void printDivider(int length = 50) {
    cout << "+";
    for (int i = 0; i < length; i++) cout << "-";
    cout << "+" << endl;
  }

  void printH1(string text) {
    cout << BG_WHITE << BLACK << text << " " << RESET << endl;
  }

  void printH2(string text) {
    cout << BOLD << text << RESET << endl;
  }

  void printSuccess(string text) {
    cout << GREEN << text << RESET << endl;
  }

  void printWarning(string text) {
    cerr << YELLOW << text << RESET << endl;
  }

  void printError(string text) {
    cerr << RED << text << RESET << endl;
  }

  void printLog(string text) {
    cerr << text << endl;
  }

  void printCheckState(string text = "check!!") {
    cerr << BG_YELLOW << text << RESET << endl;
  }
}
