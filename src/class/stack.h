#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "linkedlist.h"

using namespace std;
using namespace ll;

namespace st {
  template <typename T>
  class Stack : protected LinkedList<T> {
  public:
    Stack() : LinkedList<T>() {}
    ~Stack() {}

    T peek() {
      return this->getHead()->data;
    }

    bool isEmpty() {
      return this->getHead() == nullptr;
    }

    void push(T data) {
      this->addFront(data);
    }

    bool pop() {
      return this->removeFront();
    }
  };
}