#include <cassert>
#include <iostream>
#include <string>
#include "../src/styles/test_styles.h"
#include "../src/class/hashtable.h"

using namespace ht;
using namespace test;

const string filePath = "data/hashtable.bin";

struct Record {
  unsigned int id;
};

void testAddRecord() {
  HashTable<Record> table(5);

  table.addRecord({ 1 });
  assert(table.getRecord(1)->id == 1);
  assert(table.getRecord(2) == nullptr); 
  printSuccess("testAddRecord passed!");
}

void testRemoveRecord() {
  HashTable<Record> table(5);

  table.addRecord({ 1 });
  assert(table.getRecord(1)->id == 1);

  table.removeRecord(1);
  assert(table.getRecord(1) == nullptr); 
  printSuccess("testRemoveRecord passed!");
}

void testGetRecord() {
  HashTable<Record> table(5);

  table.addRecord({ 1 });
  table.addRecord({ 2 });

  assert(table.getRecord(1)->id == 1);
  assert(table.getRecord(2)->id == 2);
  assert(table.getRecord(3) == nullptr); 
  printSuccess("testGetRecord passed!");
}

void testSaveToBin() {
  HashTable<Record> table(5);

  table.addRecord({ 1 });
  table.addRecord({ 2 });

  table.saveToBin(filePath);

  std::ifstream file(filePath, std::ios::binary);
  assert(file.good());
  file.close();

  printSuccess("testSaveToBin passed!");
}

void testLoadFromBin() {
  HashTable<Record> table1(5);
  table1.addRecord({ 1 });
  table1.addRecord({ 2 });
  table1.saveToBin(filePath);

  HashTable<Record> table2;
  table2.loadFromBin(filePath);

  assert(table2.getRecord(1) != nullptr && table2.getRecord(1)->id == 1);
  assert(table2.getRecord(2) != nullptr && table2.getRecord(2)->id == 2);
  assert(table2.getRecord(3) == nullptr); 

  printSuccess("testLoadFromBin passed!");
}

int main() {
  cout << "Running tests..." << endl;

  testAddRecord();
  testRemoveRecord();
  testGetRecord();
  testSaveToBin();
  testLoadFromBin();

  std::cout << "All tests passed!" << std::endl;
  return 0;
}
