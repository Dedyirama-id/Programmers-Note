#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "app.h"
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
    void destroyAll() {
      delete[] table;
      table = nullptr;
      size = 0;
      capacity = 0;
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

    void addRecord(T *data) {
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
        app::printError("Error: Unable to open file for writing: " + filename);
        return false;
      }

      outFile.write(reinterpret_cast<const char *>(&size), sizeof(size));

      for (int i = 0; i < capacity; ++i) {
        if (table[i].id != EMPTY) {
          table[i].serialize(outFile);
        }
      }

      outFile.close();
      return true;
    }

    bool loadFromBin(const string &filename) {
      ifstream inFile(filename, ios::binary);
      if (!inFile) {
        return false;
      }

      int newSize;
      inFile.read(reinterpret_cast<char *>(&newSize), sizeof(newSize));
      size = newSize;

      if (table == nullptr) {
        app::printError("Error: Failed to allocate memory for table");
        return false;
      }

      // Inisialisasi tabel baru
      for (int i = 0; i < capacity; ++i) {
        table[i].id = EMPTY;
      }

      // Baca data dari file dan tambahkan ke tabel
      for (int i = 0; i < newSize; ++i) {
        T data;
        data.deserialize(inFile);
        addRecord(data);
      }

      inFile.close();
      return true;
    }

    void display() {
      for (int i = 0; i < capacity; ++i) {
        if (table[i].id != EMPTY) {
          cout << table[i].id << " " << table[i].username << " " << table[i].password << endl;
        }
      }
    }
  };
}
