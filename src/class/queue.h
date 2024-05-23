#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "linkedlist.h"

using namespace std;
using namespace ll;

namespace qu {
  template <typename T>
  class Queue : protected LinkedList<T> {
  public:
    Queue() : LinkedList<T>() {}
    ~Queue() {}

    T peek() {
      return this->getHead()->data;
    }

    bool isEmpty() {
      return this->getHead() == nullptr;
    }

    void enqueue(T data) {
      this->addBack(data);
    }

    bool dequeue() {
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
