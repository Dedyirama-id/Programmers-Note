#pragma once
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

namespace tr {
  template <typename T>
  struct Node {
    string id;
    T data;
    Node<T> *left = nullptr;
    Node<T> *right = nullptr;
    Node(string id, T value) : id(id), data(value), left(nullptr), right(nullptr) {}
    ~Node() {}
  };

  template <typename T>
  class BinaryTree {
  private:
    Node<T> *root;

    Node<T> *findPositionHelper(Node<T> *node, string id) {
      if (node == nullptr) {
        return nullptr;
      }

      if (id < node->id) {
        if (node->left == nullptr) {
          return node;
        } else {
          return findPositionHelper(node->left, id);
        }
      } else if (id > node->id) {
        if (node->right == nullptr) {
          return node;
        } else {
          return findPositionHelper(node->right, id);
        }
      } else {
        return nullptr;
      }
    }

    Node<T> *searchHelper(Node<T> *node, string id) const {
      if (node == nullptr || node->id == id) {
        return node;
      }

      if (id < node->id) {
        return searchHelper(node->left, id);
      } else {
        return searchHelper(node->right, id);
      }
    }

    Node<T> *findMin(Node<T> *node) const {
      while (node->left != nullptr) {
        node = node->left;
      }
      return node;
    }

    Node<T> *deleteNodeHelper(Node<T> *node, string id) {
      if (node == nullptr) {
        return node;
      }

      if (id < node->id) {
        node->left = deleteNodeHelper(node->left, id);
      } else if (id > node->id) {
        node->right = deleteNodeHelper(node->right, id);
      } else {
        if (node->left == nullptr) {
          Node<T> *temp = node->right;
          delete node;
          return temp;
        } else if (node->right == nullptr) {
          Node<T> *temp = node->left;
          delete node;
          return temp;
        }

        Node<T> *temp = findMin(node->right);
        node->id = temp->id;
        node->data = temp->data;
        node->right = deleteNodeHelper(node->right, temp->id);
      }
      return node;
    }

    void inOrderTraversalHelper(Node<T> *node) const {
      if (node == nullptr) {
        return;
      }
      inOrderTraversalHelper(node->left);
      cout << "[" << node->id << "] ";
      inOrderTraversalHelper(node->right);
    }

    void preOrderTraversalHelper(Node<T> *node) const {
      if (node == nullptr) {
        return;
      }
      cout << "[" << node->id << "] ";
      preOrderTraversalHelper(node->left);
      preOrderTraversalHelper(node->right);
    }

    void postOrderTraversalHelper(Node<T> *node) const {
      if (node == nullptr) {
        return;
      }
      postOrderTraversalHelper(node->left);
      postOrderTraversalHelper(node->right);
      cout << "[" << node->id << "] ";
    }

    void saveToBinHelper(ofstream &outFile, Node<T> *node) const {
      if (node == nullptr) {
        return;
      }
      outFile.write(reinterpret_cast<char *>(&node->id), sizeof(node->id));
      outFile.write(reinterpret_cast<char *>(&node->data), sizeof(node->data));
      bool hasLeft = (node->left != nullptr);
      bool hasRight = (node->right != nullptr);
      outFile.write(reinterpret_cast<char *>(&hasLeft), sizeof(hasLeft));
      outFile.write(reinterpret_cast<char *>(&hasRight), sizeof(hasRight));
      if (hasLeft) {
        saveToBinHelper(outFile, node->left);
      }
      if (hasRight) {
        saveToBinHelper(outFile, node->right);
      }
    }

    Node<T> *loadFromBinHelper(ifstream &inFile) {
      string id;
      T data;
      bool hasLeft, hasRight;
      if (inFile.read(reinterpret_cast<char *>(&id), sizeof(id))) {
        inFile.read(reinterpret_cast<char *>(&data), sizeof(data));
        Node<T> *node = new Node<T>(id, data);
        inFile.read(reinterpret_cast<char *>(&hasLeft), sizeof(hasLeft));
        inFile.read(reinterpret_cast<char *>(&hasRight), sizeof(hasRight));
        if (hasLeft) {
          node->left = loadFromBinHelper(inFile);
        }
        if (hasRight) {
          node->right = loadFromBinHelper(inFile);
        }
        return node;
      }
      return nullptr;
    }

    bool destroyAll(Node<T> *node) {
      if (node == nullptr) {
        return true;
      }
      bool leftDestroyed = destroyAll(node->left);
      bool rightDestroyed = destroyAll(node->right);
      delete node;
      node = nullptr;
      return leftDestroyed && rightDestroyed;
    }

  public:
    BinaryTree() : root(nullptr) {}

    ~BinaryTree() {
      destroyAll();
    }

    Node<T> *getRoot() { return root; }

    bool destroyAll() {
      bool allDestroyed = destroyAll(root);
      root = nullptr; 
      return allDestroyed;
    }


    Node<T> *findPosition(string id) {
      if (root == nullptr) {
        return nullptr;
      }
      return findPositionHelper(root, id);
    }

    bool insert(string id, T value) {
      Node<T> *parent = findPosition(id);
      if (parent == nullptr) {
        if (root == nullptr) {
          root = new Node<T>(id, value);
          return true;
        } else {
          return false;
        }
      } else {
        if (id < parent->id) {
          parent->left = new Node<T>(id, value);
        } else {
          parent->right = new Node<T>(id, value);
        }
        return true;
      }
    }

    Node<T> *search(string id) const {
      return searchHelper(root, id);
    }

    void inOrderTraversal() const {
      inOrderTraversalHelper(root);
      cout << endl;
    }

    void preOrderTraversal() const {
      preOrderTraversalHelper(root);
      cout << endl;
    }

    void postOrderTraversal() const {
      postOrderTraversalHelper(root);
      cout << endl;
    }

    void deleteNode(string id) {
      root = deleteNodeHelper(root, id);
    }

    void saveToBin(const string &filename) const {
      ofstream outFile(filename, ios::binary);
      if (!outFile) {
        throw runtime_error("Unable to open file for reading: " + filename);
        return;
      }
      saveToBinHelper(outFile, root);
      outFile.close();
    }

    void loadFromBin(const string &filename) {
      ifstream inFile(filename, ios::binary);
      if (!inFile) {
        throw runtime_error("Unable to open file for reading: " + filename);
        return;
      }

      destroyAll();
      root = loadFromBinHelper(inFile);
      inFile.close();
    }
  };
}
