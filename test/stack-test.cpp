#include <cassert>
#include <iostream>
#include "../src/styles/test_styles.h"
#include "../src/class/stack.h"

using namespace std;
using namespace test;
using namespace st;

const string filePath = "data/stack.bin";
const string emptyTestFilePath = "data/empty.bin";

void testPush() {
  Stack<int> stack;
  stack.push(1);
  assert(stack.peek() == 1);
  stack.push(2);
  assert(stack.peek() == 2);
  stack.push(3);
  assert(stack.peek() == 3);
  printSuccess("testPush passed!");
}

void testPop() {
  Stack<int> stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);
  stack.pop();
  assert(stack.peek() == 2);
  stack.pop();
  assert(stack.peek() == 1);
  stack.pop();
  assert(stack.isEmpty());
  printSuccess("testPop passed!");
}

void testPeek() {
  Stack<int> stack;
  stack.push(1);
  assert(stack.peek() == 1);
  stack.push(2);
  assert(stack.peek() == 2);
  stack.pop();
  assert(stack.peek() == 1);
  printSuccess("testPeek passed!");
}

void testIsEmpty() {
  Stack<int> stack;
  assert(stack.isEmpty());
  stack.push(1);
  assert(!stack.isEmpty());
  stack.pop();
  assert(stack.isEmpty());
  printSuccess("testIsEmpty passed!");
}

void testSaveAndLoadStack() {
  Stack<int> stack;
  stack.push(1);
  stack.push(2);
  stack.push(3);

  bool saveResult = stack.saveToBin(filePath);
  assert(saveResult);

  while (!stack.isEmpty()) {
    stack.pop();
  }

  bool loadResult = stack.loadFromBin(filePath);
  assert(loadResult);

  assert(stack.peek() == 3);
  stack.pop();
  assert(stack.peek() == 2);
  stack.pop();
  assert(stack.peek() == 1);
  stack.pop();
  assert(stack.isEmpty());

  printSuccess("testSaveAndLoadStack passed!");
}

void testSaveAndLoadEmptyStack() {
  Stack<int> stack;

  bool saveResult = stack.saveToBin(emptyTestFilePath);
  assert(saveResult);

  while (!stack.isEmpty()) {
    stack.pop();
  }

  bool loadResult = stack.loadFromBin(emptyTestFilePath);
  assert(loadResult);

  assert(stack.isEmpty());

  printSuccess("testSaveAndLoadEmptyStack passed!");
}

int main() {
  cout << "Running tests..." << endl;

  testPush();
  testPop();
  testPeek();
  testIsEmpty();
  testSaveAndLoadStack();
  testSaveAndLoadEmptyStack();

  cout << "All tests passed!" << endl;
  return 0;
}
