#include <cassert>
#include <iostream>
#include <fstream>
#include "../src/styles/test_styles.h"
#include "../src/class/binarytree.h"

using namespace std;
using namespace test;
using namespace tr;

const string filename = "data/tree.bin";

void testInsert() {
  BinaryTree<int> tree;
  tree.insert(5, 5);
  assert(tree.search(5));
  tree.insert(3, 3);
  assert(tree.search(3));
  tree.insert(8, 8);
  assert(tree.search(8));
  tree.insert(1, 1);
  assert(tree.search(1));
  printSuccess("testInsert passed!");
}

void testDelete() {
  BinaryTree<int> tree;
  tree.insert(5, 5);
  tree.insert(3, 3);
  tree.insert(8, 8);
  tree.insert(1, 1);
  tree.insert(7, 7);
  tree.deleteNode(1);
  assert(!tree.search(1));
  tree.deleteNode(8);
  assert(!tree.search(8));
  printSuccess("testDelete passed!");
}

void testSearch() {
  BinaryTree<int> tree;
  tree.insert(5, 5);
  tree.insert(3, 3);
  tree.insert(8, 8);
  tree.insert(1, 1);
  tree.insert(7, 7);
  assert(tree.search(5));
  assert(tree.search(3));
  assert(tree.search(8));
  assert(tree.search(1));
  assert(tree.search(7));
  assert(!tree.search(10));
  printSuccess("testSearch passed!");
}

void testTraversal() {
  BinaryTree<int> tree;
  tree.insert(5, 5);
  tree.insert(3, 3);
  tree.insert(8, 8);
  tree.insert(1, 1);
  tree.insert(7, 7);

  printWarning("Check this traversal result: ");
  cout << "- Inorder traversal: ";
  tree.inOrderTraversal();

  cout << "- Preorder traversal: ";
  tree.preOrderTraversal();

  cout << "- Postorder traversal: ";
  tree.postOrderTraversal();

  printSuccess("testTraversal nothing error!");
}

void testSaveAndLoad() {
  BinaryTree<int> tree;
  tree.insert(5, 5);
  tree.insert(3, 3);
  tree.insert(8, 8);
  tree.insert(1, 1);
  tree.insert(7, 7);

  tree.saveToBin(filename);
  printWarning("Compare this traversal result: ");
  cout << "- Saved tree Inorder traversal: ";
  tree.inOrderTraversal();

  BinaryTree<int> loadedTree;
  loadedTree.loadFromBin(filename);

  assert(loadedTree.search(5));
  assert(loadedTree.search(3));
  assert(loadedTree.search(8));
  assert(loadedTree.search(1));
  assert(loadedTree.search(7));
  assert(!loadedTree.search(10));

  cout << "- Loaded tree Inorder traversal: ";
  loadedTree.inOrderTraversal();

  printSuccess("testSaveAndLoad nothing error!");
}

int main() {
  cout << "Running tests..." << endl;

  testInsert();
  testDelete();
  testSearch();
  testTraversal();
  testSaveAndLoad();

  cout << "All tests passed!" << endl;
  return 0;
}
