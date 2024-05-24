#pragma once
#include <iostream>
#include <string>
#include <fstream>
#define EMPTY 0
#define MAX_SCALE 0.7

using namespace std;

namespace ht {
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
      destroyAll();
    }

    bool isEmpty() const {
      return size == 0;
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

      destroyAll();
      table = newTable;
    }

  public:
    destroyAll() {
      delete[] table;
    }

    void addRecord(const T data) {
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

    void addRecord (T *data) {
      addRecord(*data);
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
          break;
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
          break;
        }
      }

      return nullptr;
    }

    bool saveToBin(const string &filename) const {
      ofstream outFile(filename, ios::binary);
      if (!outFile) {
        // throw runtime_error("Unable to open file for writing" + filename);
        return false;
      }

      outFile.write(reinterpret_cast<const char *>(&capacity), sizeof(capacity));
      outFile.write(reinterpret_cast<const char *>(&size), sizeof(size));

      for (int i = 0; i < capacity; ++i) {
        outFile.write(reinterpret_cast<const char *>(&table[i]), sizeof(T));
      }

      outFile.close();
    }

    bool loadFromBin(const string &filename) {
      ifstream inFile(filename, ios::binary);
      if (!inFile) {
        // throw runtime_error("Unable to open file for reading: " + filename);
        return false;
      }

      inFile.read(reinterpret_cast<char *>(&capacity), sizeof(capacity));
      inFile.read(reinterpret_cast<char *>(&size), sizeof(size));

      destroyAll();
      table = new T[capacity];

      for (int i = 0; i < capacity; ++i) {
        inFile.read(reinterpret_cast<char *>(&table[i]), sizeof(T));
      }

      inFile.close();
      return true;
    }
  };
}
