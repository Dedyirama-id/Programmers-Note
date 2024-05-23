#include <cassert>
#include <iostream>
#include "../src/styles/test_styles.h"
#include "../src/class/queue.h"

using namespace std;
using namespace test;
using namespace qu;

const string filePath = "data/queue.bin";
const string emptyTestFilePath = "data/empty.bin";

void testEnqueue() {
  Queue<int> queue;
  queue.enqueue(1);
  assert(queue.peek() == 1);
  queue.enqueue(2);
  assert(queue.peek() == 1);
  queue.enqueue(3);
  assert(queue.peek() == 1);
  printSuccess("testEnqueue passed!");
}

void testDequeue() {
  Queue<int> queue;
  queue.enqueue(1);
  queue.enqueue(2);
  queue.enqueue(3);
  assert(queue.peek() == 1);
  queue.dequeue();
  assert(queue.peek() == 2);
  queue.dequeue();
  assert(queue.peek() == 3);
  queue.dequeue();
  assert(queue.isEmpty());
  printSuccess("testDequeue passed!");
}

void testPeek() {
  Queue<int> queue;
  queue.enqueue(1);
  assert(queue.peek() == 1);
  queue.enqueue(2);
  assert(queue.peek() == 1);
  queue.dequeue();
  assert(queue.peek() == 2);
  printSuccess("testPeek passed!");
}

void testIsEmpty() {
  Queue<int> queue;
  assert(queue.isEmpty());
  queue.enqueue(1);
  assert(!queue.isEmpty());
  queue.dequeue();
  assert(queue.isEmpty());
  printSuccess("testIsEmpty passed!");
}

void testSaveAndLoadQueue() {
  Queue<int> queue;
  queue.enqueue(1);
  queue.enqueue(2);
  queue.enqueue(3);

  bool saveResult = queue.saveToBin(filePath);
  assert(saveResult);

  while (!queue.isEmpty()) {
    queue.dequeue();
  }

  bool loadResult = queue.loadFromBin(filePath);
  assert(loadResult);

  assert(queue.peek() == 1);
  queue.dequeue();
  assert(queue.peek() == 2);
  queue.dequeue();
  assert(queue.peek() == 3);
  queue.dequeue();
  assert(queue.isEmpty());

  printSuccess("testSaveAndLoadQueue passed!");
}

void testSaveAndLoadEmptyQueue() {
  Queue<int> queue;

  bool saveResult = queue.saveToBin(emptyTestFilePath);
  assert(saveResult);

  while (!queue.isEmpty()) {
    queue.dequeue();
  }

  bool loadResult = queue.loadFromBin(emptyTestFilePath);
  assert(loadResult);

  assert(queue.isEmpty());

  printSuccess("testSaveAndLoadEmptyQueue passed!");
}

int main() {
  cout << "Running tests..." << endl;

  testEnqueue();
  testDequeue();
  testPeek();
  testIsEmpty();
  testSaveAndLoadQueue();
  testSaveAndLoadEmptyQueue();

  cout << "All tests passed!" << endl;
  return 0;
}
