#pragma once
#include <string>

using namespace std;

struct Note {
  string title;
  string content[10];
  int count = 0;

  Note(string title) : title(title) {}
  Note(string title, string content) : title(title) {
    this->content[0] = content;
  }

  bool isEmpty() { return title == ""; }
  bool isFull() { return count == 10; }

  bool addContent(string content) {
    if (isFull()) return false;

    this->content[count] = content;
    count++;

    return true;
  }

  bool deleteContent(int index) {
    if (isEmpty()) return false;

    for (int i = index; i < count - 1; i++) {
      this->content[i] = this->content[i + 1];
    }
    count--;
    return true;
  }

  string getContent(int index) {
    if (isEmpty()) return "";
    return this->content[index];
  }
};