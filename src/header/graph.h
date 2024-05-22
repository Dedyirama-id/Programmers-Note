#pragma once
#include <iostream>
#include <string>
#include "linkedlist.h"

using namespace std;

namespace gr {
  template <typename T> class Vertex;

  template <typename T>
  struct Edge {
    Vertex<T> *vertexRef;
    Edge<T> *next;

    Edge(Vertex<T> *vertexRef) : vertexRef(vertexRef), next(nullptr) {}
  };

  template <typename T>
  struct Vertex : public ll::Node<T> {
    ll::LinkedList<Edge<T>> *edgeList;

    Vertex(T data) : ll::Node<T>(data), edgeList(new ll::LinkedList<Edge<T>>()) {}

    void add2WayEdgeByVertex(Vertex<T> *vertex) {
      edgeList->addBack(Edge<T>(vertex));
    }
  };

  template <typename T>
  class Graph : public ll::LinkedList<T> {
  public:
    string filePath;

    Graph(string filePath) : filePath(filePath) {}

    bool checkIfEdgeExistByID(unsigned int sourceVID, unsigned int destVID) {
      if (this->isEmpty()) return false;

      Vertex<T> *sourceNode = this->searchById(sourceVID);
      if (!sourceNode) return false;
      Vertex<T> *source = &(sourceNode->data);
      return source->edgeList->searchById(destVID) != nullptr;
    }

    bool addEdgeByID(unsigned int sourceVID, unsigned int destVID) {
      ll::Node<Vertex<T>> *sourceNode = this->searchById(sourceVID);
      ll::Node<Vertex<T>> *destNode = this->searchById(destVID);

      if (!sourceNode || !destNode) return false;

      Vertex<T> *sourceVertex = &(sourceNode->data);
      Vertex<T> *destVertex = &(destNode->data);

      if (!checkIfEdgeExistByID(sourceVID, destVID)) {
        sourceNode->data.edgeList->addBack(Edge<T>(destVertex));
        destNode->data.edgeList->addBack(Edge<T>(sourceVertex));
        return true;
      }
      return false;
    }

    bool addVertex(T data) {
      this->addBack(Vertex<T>(data));
      return true;
    }

    void displayGraph() {
      Vertex<T> *vertexNode = this->getHead();
      while (vertexNode != nullptr) {
        cout << vertexNode->data.data << " -> ";
        Edge<T> *edgeNode = vertexNode->data.edgeList->getHead();
        while (edgeNode != nullptr) {
          cout << edgeNode->data.vertexRef->data << " "; // Akses langsung data dari vertexRef
          edgeNode = edgeNode->next;
        }
        cout << endl;
        vertexNode = vertexNode->next;
      }
    }
  };
}
