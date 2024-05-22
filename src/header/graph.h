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
    Edge<T> *prev = nullptr;
    Edge<T> *next = nullptr;

    Edge(Vertex<T> *vertexRef) : vertexRef(vertexRef), next(nullptr) {}
  };

  template <typename T>
  struct Vertex {
    unsigned int id;
    T data;
    Vertex<T> *next = nullptr;
    Vertex<T> *prev = nullptr;
    Edge<T> *edgeList = nullptr;

    Vertex(T data) : data(data) {}

    bool addEdgeByVertex(Vertex<T> *vertex) {
      if (edgeList == nullptr) {
        edgeList = new Edge<T>(vertex);
        return true;
      }
      
      Edge<T> *newEdge = new Edge<T>(vertex);
      newEdge->next = edgeList;
      edgeList->prev = newEdge;
      edgeList = newEdge;
      return true;
    }

    bool addEdgeByVertexId(unsigned int id) {
      return addEdgeByVertex(searchById(id));
    }
  };

  template <typename T>
  class Graph {
  public:
    Vertex<T> *vertices;
    int vertexCount;
    int edgeCount;
    string filePath;

    Graph() : vertices(nullptr), vertexCount(0), edgeCount(0) {}
    Graph(string filePath) : vertices(nullptr), vertexCount(0), edgeCount(0) {}

    ~Graph() {}

    bool isEmpty() { return vertices == nullptr; }

    bool addVertex(Vertex<T> *newVertex) {
      if (isEmpty()) {
        vertices = newVertex;
        vertexCount++;
        return true;
      }

      newVertex->next = vertices;
      vertices->prev = newVertex;
      vertices = newVertex;
      vertexCount++;
      return true;
    }

    bool addVertex(T data) {
      Vertex<T> *newVertex = new Vertex<T>(data);
      return addVertex(newVertex);
    }

    Vertex<T> *searchById(int id) {
      Vertex<T> *current = vertices;
      while (current != nullptr) {
        if (current->id == id) {
          return current;
        }
        current = current->next;
      }
      return nullptr;
    }

    bool deleteVertexByPtr(Vertex<T> *vertex) {
      if (vertex == nullptr) return false;
      if (vertex->prev != nullptr) {
        vertex->prev->next = vertex->next;
      }
      if (vertex->next != nullptr) {
        vertex->next->prev = vertex->prev;
      }
      delete vertex;
      return true;
    }

    bool deleteVertexById(unsigned int id) {
      Vertex<T> *vertex = searchById(id);
      if (vertex == nullptr) return false;
      return deleteVertexByPtr(vertex);
    }

    bool addEdgeByID(unsigned int sourceID, unsigned int destID) {
      Vertex<T> *sourceVertex = searchById(sourceID);
      Vertex<T> *destVertex = searchById(destID);
      if (sourceVertex == nullptr || destVertex == nullptr) return false;
      sourceVertex->edgeList = new Edge<T>(destVertex);
      return true;
    }

    void displayGraph() {
      Vertex<T> *current = vertices;
      while (current != nullptr) {
        cout << current->data << " ";
        current = current->next;
      }
      cout << endl;
    }
  };
}
