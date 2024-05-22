#pragma once
#include <iostream>
#include <string>
#include <fstream>
#define EMPTY 0
#define MAX_SCALE 0.7

namespace ml {
  template <typename T>
  class HashTable {
    T *table;
    int capacity;
    int size;

  public:
    HashTable(int capacity = 10) : capacity(capacity), size(0) {
      table = new T[capacity];
      for (int i = 0; i < capacity; ++i) {
        table[i].id = EMPTY;
      }
    }

    ~HashTable() {
      delete[] table;
    }

  private:
    int hashFunction(unsigned int id) const {
      return id % capacity;
    }

    void rehash() {
      int oldCapacity = capacity;
      capacity *= 2;
      T *newTable = new T[capacity];

      for (int i = 0; i < capacity; ++i) {
        newTable[i].id = EMPTY;
      }

      for (int i = 0; i < oldCapacity; ++i) {
        if (table[i].id != EMPTY) {
          int newIndex = hashFunction(table[i].id);
          while (newTable[newIndex].id != EMPTY) {
            newIndex = (newIndex + 1) % capacity;
          }
          newTable[newIndex] = table[i];
        }
      }

      delete[] table;
      table = newTable;
    }

  public:
    void addRecord(const T &data) {
      if (size > capacity * MAX_SCALE) {
        rehash();
      }

      int index = hashFunction(data.id);
      while (table[index].id != EMPTY && table[index].id != data.id) {
        index = (index + 1) % capacity;
      }

      if (table[index].id == EMPTY) {
        ++size;
      }

      table[index] = data;
    }

    bool removeRecord(unsigned int id) {
      int index = hashFunction(id);
      int startIndex = index;

      while (table[index].id != EMPTY) {
        if (table[index].id == id) {
          table[index].id = EMPTY;
          --size;
          return true;
        }
        index = (index + 1) % capacity;
        if (index == startIndex) {
          break; // Prevent infinite loop
        }
      }

      return false;
    }

    T *getRecord(unsigned int id) {
      int index = hashFunction(id);
      int startIndex = index;

      while (table[index].id != EMPTY) {
        if (table[index].id == id) {
          return &table[index];
        }
        index = (index + 1) % capacity;
        if (index == startIndex) {
          break; // Prevent infinite loop
        }
      }

      return nullptr;
    }
  };
}
