#include <cassert>
#include <iostream>
#include "../src/class/linkedlist.h"
#include "../src/styles/test_styles.h"

using namespace ll;
using namespace test;

const string filePath = "data/linkedlist.bin";
const string emptyTestFilePath = "data/empty.bin";

void testAddBack() {
  LinkedList<int> list;
  assert(list.isEmpty());

  list.addBack(1);
  assert(list.getHead()->data == 1);
  assert(list.getTail()->data == 1);
  assert(list.count == 1);

  list.addBack(2);
  assert(list.getHead()->data == 1);
  assert(list.getTail()->data == 2);
  assert(list.count == 2);

  printSuccess("testAddBack passed!");
}

void testAddFront() {
  LinkedList<int> list;
  assert(list.isEmpty());

  list.addFront(1);
  assert(list.getHead()->data == 1);
  assert(list.getTail()->data == 1);
  assert(list.count == 1);

  list.addFront(2);
  assert(list.getHead()->data == 2);
  assert(list.getTail()->data == 1);
  assert(list.count == 2);

  printSuccess("testAddFront passed!");
}

void testRemoveBack() {
  LinkedList<int> list;
  list.addBack(1);
  list.addBack(2);
  list.addBack(3);
  assert(list.count == 3);

  list.removeBack();
  assert(list.getTail()->data == 2);
  assert(list.count == 2);

  list.removeBack();
  assert(list.getTail()->data == 1);
  assert(list.count == 1);

  list.removeBack();
  assert(list.isEmpty());

  printSuccess("testRemoveBack passed!");
}

void testRemoveFront() {
  LinkedList<int> list;
  list.addFront(1);
  list.addFront(2);
  list.addFront(3);
  assert(list.count == 3);

  list.removeFront();
  assert(list.getHead()->data == 2);
  assert(list.count == 2);

  list.removeFront();
  assert(list.getHead()->data == 1);
  assert(list.count == 1);

  list.removeFront();
  assert(list.isEmpty());

  printSuccess("testRemoveFront passed!");
}

void testSearchById() {
  LinkedList<int> list;
  Node<int> *node1 = new Node<int>(1);
  node1->id = 1;
  Node<int> *node2 = new Node<int>(2);
  node2->id = 2;
  list.addBack(node1);
  list.addBack(node2);

  assert(list.searchById(1) == node1);
  assert(list.searchById(2) == node2);
  assert(list.searchById(3) == nullptr);

  printSuccess("testSearchById passed!");
}

void testSearchByData() {
  LinkedList<int> list;
  list.addBack(1);
  list.addBack(2);
  list.addBack(3);

  assert(list.searchByData(1)->data == 1);
  assert(list.searchByData(2)->data == 2);
  assert(list.searchByData(3)->data == 3);
  assert(list.searchByData(4) == nullptr);

  printSuccess("testSearchByData passed!");
}

void testDeleteByPtr() {
  LinkedList<int> list;
  Node<int> *node1 = new Node<int>(1);
  Node<int> *node2 = new Node<int>(2);
  Node<int> *node3 = new Node<int>(3);
  list.addBack(node1);
  list.addBack(node2);
  list.addBack(node3);

  assert(list.deleteByPtr(node2));
  assert(list.searchByData(2) == nullptr);
  assert(list.count == 2);

  assert(list.deleteByPtr(node1));
  assert(list.searchByData(1) == nullptr);
  assert(list.count == 1);

  assert(list.deleteByPtr(node3));
  assert(list.searchByData(3) == nullptr);
  assert(list.count == 0);
  assert(list.isEmpty());

  printSuccess("testDeleteByPtr passed!");
}

void testDeleteByData() {
  LinkedList<int> list;
  list.addBack(1);
  list.addBack(2);
  list.addBack(3);

  assert(list.deleteByData(2));
  assert(list.searchByData(2) == nullptr);
  assert(list.count == 2);

  assert(list.deleteByData(1));
  assert(list.searchByData(1) == nullptr);
  assert(list.count == 1);

  assert(list.deleteByData(3));
  assert(list.searchByData(3) == nullptr);
  assert(list.count == 0);
  assert(list.isEmpty());

  printSuccess("testDeleteByData passed!");
}

void testSaveAndLoad() {
  LinkedList<int> list;
  list.addBack(1);
  list.addBack(2);
  list.addBack(3);

  bool saveResult = list.saveToBin(filePath);
  assert(saveResult);

  while (!list.isEmpty()) {
    list.removeFront();
  }

  bool loadResult = list.loadFromBin(filePath);
  assert(loadResult);

  assert(list.getHead()->data == 1);
  assert(list.getTail()->data == 3);
  assert(list.count == 3);

  printSuccess("testSaveAndLoad passed!");
}

void testSaveAndLoadEmptyList() {
  LinkedList<int> list;

  bool saveResult = list.saveToBin(emptyTestFilePath);
  assert(saveResult);

  while (!list.isEmpty()) {
    list.removeFront();
  }

  bool loadResult = list.loadFromBin(emptyTestFilePath);
  assert(loadResult);

  assert(list.isEmpty());

  printSuccess("testSaveAndLoadEmptyList passed!");
}

int main() {
  cout << "Running tests..." << endl;

  testAddBack();
  testAddFront();
  testRemoveBack();
  testRemoveFront();
  testSearchById();
  testSearchByData();
  testDeleteByPtr();
  testDeleteByData();
  testSaveAndLoad();
  testSaveAndLoadEmptyList();

 cout << "All tests passed!" <<endl;
  return 0;
}
