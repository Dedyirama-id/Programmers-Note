#pragma once
#include <iostream>
#include <string>
#include "app.h"

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

    Vertex(T data, unsigned int id) : data(data), id(id) {}
    ~Vertex() {
      if (edgeList != nullptr) {
        Edge<T> *toDelete = edgeList;
        while (toDelete != nullptr) {
          edgeList = edgeList->next;
          delete toDelete;
          toDelete = edgeList;
        }
        edgeList = nullptr;
      }
    }

    Edge<T> *searchEdgeById(unsigned int id) {
      Edge<T> *current = edgeList;
      while (current != nullptr) {
        if (current->vertexRef->id == id) {
          return current;
        }
        current = current->next;
      }
      return nullptr;
    }

    bool addEdgeByVertex(Vertex<T> *vertex) {
      if (edgeList == nullptr) {
        edgeList = new Edge<T>(vertex);
        return true;
      }

      Edge<T> *newEdge = new Edge<T>(vertex);
      newEdge->next = edgeList;
      edgeList->prev = newEdge;
      edgeList = newEdge;
      newEdge->prev = nullptr;
      return true;
    }

    bool addEdgeByVertexId(unsigned int id) {
      Vertex<T> *vertex = searchEdgeById(id);
      if (vertex == nullptr) return false;
      return addEdgeByVertex(vertex);
    }

    bool deleteEdge(Edge<T> *edge) {
      if (edge == nullptr) return false;
      if (edge->prev != nullptr) {
        edge->prev->next = edge->next;
      }
      if (edge->next != nullptr) {
        edge->next->prev = edge->prev;
      }
      delete edge;
      return true;
    }

    bool deleteEdgeById(unsigned int id) {
      Edge<T> *edge = searchEdgeById(id);
      if (edge == nullptr) return false;
      return deleteEdge(edge);
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
    Graph(string filePath) : vertices(nullptr), vertexCount(0), edgeCount(0), filePath(filePath) {}

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

    bool addVertex(T data, unsigned int id) {
      Vertex<T> *newVertex = new Vertex<T>(data, id);
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

    bool destroyRelationshipToVertex(unsigned int id) {
      Vertex<T> *current = vertices;
      while (current != nullptr) {
        Edge<T> *connectedEdge = current->searchEdgeById(id);
        if (connectedEdge != nullptr) {
          current->deleteEdge(connectedEdge);
        }
        current = current->next;
      }
      return true;
    }

    bool deleteVertexByPtr(Vertex<T> *vertex) {
      if (vertex == nullptr) return false;
      if (vertex->edgeList != nullptr) destroyRelationshipToVertex(vertex->id);

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

    bool addEdgeById(unsigned int sourceId, unsigned int destID) {
      Vertex<T> *sourceVertex = searchById(sourceId);
      Vertex<T> *destVertex = searchById(destID);
      if (sourceVertex == nullptr || destVertex == nullptr) return false;
      sourceVertex->addEdgeByVertex(destVertex);
      return true;
    }

    void displayGraph() {
      Vertex<T> *current = vertices;
      while (current != nullptr) {
        cout << "[" << current->id << "] " << current->data << " -> ";
        Edge<T> *currentEdge = current->edgeList;
        while (currentEdge != nullptr) {
          cout << currentEdge->vertexRef->id;
          if (currentEdge->next != nullptr) {
            cout << ", ";
          }
          currentEdge = currentEdge->next;
        }
        cout << endl;
        current = current->next;
      }
    }
  };
}
