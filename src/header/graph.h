#pragma once
#include <iostream>
#include <string>
#include <fstream>
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
    int degree;
    T data;
    Vertex<T> *next = nullptr;
    Vertex<T> *prev = nullptr;
    Edge<T> *edgeList = nullptr;

    Vertex(T data, unsigned int id) : data(data), id(id), degree(0) {}
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
        degree++;
        return true;
      }

      Edge<T> *newEdge = new Edge<T>(vertex);
      newEdge->next = edgeList;
      edgeList->prev = newEdge;
      edgeList = newEdge;
      newEdge->prev = nullptr;
      degree++;
      return true;
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
    string filePath;

    Graph() : vertices(nullptr), vertexCount(0), filePath("") {}
    Graph(string filePath) : vertices(nullptr), vertexCount(0), filePath(filePath) {
      // if (filePath != "") loadFromBin();
    }

    ~Graph() {
      // saveToBin();
      if (vertices != nullptr) {
        Vertex<T> *current = vertices;
        while (current != nullptr) {
          Vertex<T> *toDelete = current;
          current = current->next;
          delete toDelete;
        }
      }
    }

    bool isEmpty() { return vertices == nullptr; }

    bool addVertex(Vertex<T> *newVertex) {
      if (isEmpty()) {
        vertices = newVertex;
      } else {
        newVertex->next = vertices;
        vertices->prev = newVertex;
        vertices = newVertex;
      }
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

    bool saveToBin(string filePath = "") {
      if (filePath == "") {
        filePath = this->filePath;
      }

      ofstream ofs(filePath, ios::binary | ios::out | ios::trunc);
      if (!ofs.is_open()) {
        return false;
      }

      ofs.write(reinterpret_cast<char *>(&vertexCount), sizeof(vertexCount));

      Vertex<T> *currentVertex = vertices;
      while (currentVertex != nullptr) {
        ofs.write(reinterpret_cast<char *>(&currentVertex->id), sizeof(currentVertex->id));
        ofs.write(reinterpret_cast<char *>(&currentVertex->degree), sizeof(currentVertex->degree));
        ofs.write(reinterpret_cast<char *>(&currentVertex->data), sizeof(currentVertex->data));

        currentVertex = currentVertex->next;
      }

      currentVertex = vertices;
      while (currentVertex != nullptr) {
        Edge<T> *currentEdge = currentVertex->edgeList;
        while (currentEdge != nullptr) {
          ofs.write(reinterpret_cast<char *>(&currentEdge->vertexRef->id), sizeof(currentEdge->vertexRef->id));
          currentEdge = currentEdge->next;
        }
        currentVertex = currentVertex->next;
      }

      ofs.close();
      return true;
    }

    bool loadFromBin(string filePath = "") {
      if (filePath == "") {
        filePath = this->filePath;
      }

      ifstream ifs(filePath, ios::binary);
      if (!ifs.is_open()) {
        return false;
      }

      int vertexCount;
      ifs.read(reinterpret_cast<char *>(&vertexCount), sizeof(vertexCount));
      this->vertexCount = vertexCount;

      for (int i = 0; i < vertexCount; ++i) {
        unsigned int id;
        ifs.read(reinterpret_cast<char *>(&id), sizeof(id));

        T data;
        ifs.read(reinterpret_cast<char *>(&data), sizeof(data));

        Vertex<T> *newVertex = new Vertex<T>(data, id);
        addVertex(newVertex);
      }

      Vertex<T> *currentVertex = vertices;
      while (currentVertex != nullptr) {
        unsigned int id;
        ifs.read(reinterpret_cast<char *>(&id), sizeof(id));
        addEdgeById(currentVertex->id, id);
        currentVertex = currentVertex->next;
      }
      

      ifs.close();
      return true;
    }

  };
}
