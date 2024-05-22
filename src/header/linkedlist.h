#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

namespace ll {
  template <typename T>
  struct Node {
    unsigned int id;
    T data;
    Node<T> *next;
    Node<T> *prev;
    Node(T data) : data(data), id(0), next(nullptr), prev(nullptr) {}
  };

  template <typename T>
  class LinkedList {
    Node<T> *head;
    Node<T> *tail;

  public:
    int count = 0;

    LinkedList() : head(nullptr), tail(nullptr) {}

    ~LinkedList() {
      Node<T> *current = head;
      while (current != nullptr) {
        Node<T> *toDelete = current;
        current = current->next;
        delete toDelete;
      }
    }

    Node<T> *getHead() { return head; }
    Node<T> *getTail() { return tail; }

    bool isEmpty() { return head == nullptr; }

    bool addBack(Node<T> *newNode) {
      if (newNode == nullptr) {
        return false;
      }

      count++;
      if (head == nullptr) {
        head = newNode;
        tail = newNode;
        return true;
      }
      tail->next = newNode;
      newNode->prev = tail;
      tail = newNode;
      return true;
    }

    bool addBack(T data) {
      Node<T> *newNode = new Node<T>(data);
      return addBack(newNode);
    }

    bool addFront(Node<T> *newNode) {
      if (newNode == nullptr) {
        return false;
      }

      count++;
      if (head == nullptr) {
        head = newNode;
        tail = newNode;
        return true;
      }
      head->prev = newNode;
      newNode->next = head;
      head = newNode;
      return true;
    }

    bool addFront(T data) {
      Node<T> *newNode = new Node<T>(data);
      return addFront(newNode);
    }

    bool removeBack() {
      if (head == nullptr) {
        return false;
      }

      count--;
      if (head == tail) {
        delete head;
        head = nullptr;
        tail = nullptr;
        return true;
      }
      tail = tail->prev;
      tail->next = nullptr;
      delete tail->next;
      return true;
    }

    bool removeFront() {
      if (head == nullptr) {
        return false;
      }

      count--;
      if (head == tail) {
        delete head;
        head = nullptr;
        tail = nullptr;
        return true;
      }
      head = head->next;
      head->prev = nullptr;
      delete head->prev;
      return true;
    }

    Node<T> *searchById(unsigned int id) {
      Node<T> *current = head;
      while (current != nullptr) {
        if (current->id == id) {
          return current;
        }
        current = current->next;
      }
      return nullptr;
    }

    Node<T> *searchByData(T data) {
      Node<T> *current = head;
      while (current != nullptr) {
        if (current->data == data) {
          return current;
        }
        current = current->next;
      }
      return nullptr;
    }

    bool deleteByPtr(Node<T> *toDelete) {
      if (toDelete == head && toDelete == tail) {
        delete head;
        head = nullptr;
        tail = nullptr;
        return true;
      } else if (toDelete == head) {
        head = head->next;
        head->prev = nullptr;
        delete toDelete;
        return true;
      } else if (toDelete == tail) {
        tail = tail->prev;
        tail->next = nullptr;
        delete toDelete;
        return true;
      }

      toDelete->prev->next = toDelete->next;
      toDelete->next->prev = toDelete->prev;
      delete toDelete;
      return true;
    }

    bool deleteByData(T data) {
      Node<T> *toPop = searchByData(data);
      if (toPop == nullptr) {
        return false;
      }
      return deleteByPtr(toPop);
    }

    bool deleteById(unsigned int id) {
      Node<T> *toPop = searchById(id);
      if (toPop == nullptr) {
        return false;
      }
      return deleteByPtr(toPop);
    }

    void display(string separator = " ") {
      Node<T> *current = head;
      while (current != tail) {
        cout << current->data << separator;
        current = current->next;
      }
      cout << tail->data << endl;
    }
  };
}