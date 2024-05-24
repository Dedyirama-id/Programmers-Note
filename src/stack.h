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
    ~Stack() {
      destroyAll();
    }

    bool destroyAll() {
      return LinkedList<T>::destroyAll();
    }

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

    bool saveToBin(const string &filename) {
      return LinkedList<T>::saveToBin(filename);
    }

    bool loadFromBin(const string &filename) {
      return LinkedList<T>::loadFromBin(filename);
    }
  };
}