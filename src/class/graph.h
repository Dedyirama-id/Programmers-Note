#include <iostream>
#include <string>
#include <fstream>
#include "../app/app.h"

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
    Vertex(T data, unsigned int id, int degree) : data(data), id(id), degree(degree) {}
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

      degree--;
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
    Vertex<T> *verticesHead;
    Vertex<T> *verticesTail;
  public:
    int vertexCount;

    Graph() : verticesHead(nullptr), verticesTail(nullptr), vertexCount(0) {}

    ~Graph() {
      if (verticesHead != nullptr) {
        Vertex<T> *current = verticesHead;
        while (current != nullptr) {
          Vertex<T> *toDelete = current;
          current = current->next;
          delete toDelete;
        }
      }
    }

    Vertex<T> *getVerticesHead() const { return verticesHead; }
    Vertex<T> *getVerticesTail() const { return verticesTail; }

    bool isEmpty() { return verticesHead == nullptr; }

    bool addVertex(Vertex<T> *newVertex) {
      if (isEmpty()) {
        verticesHead = newVertex;
        verticesTail = newVertex;
      } else {
        verticesTail->next = newVertex;
        newVertex->prev = verticesTail;
        verticesTail = newVertex;
      }
      vertexCount++;
      return true;
    }

    bool addVertex(T data, unsigned int id) {
      Vertex<T> *newVertex = new Vertex<T>(data, id);
      return addVertex(newVertex);
    }

    Vertex<T> *searchById(int id) {
      Vertex<T> *current = verticesHead;
      while (current != nullptr) {
        if (current->id == id) {
          return current;
        }
        current = current->next;
      }
      return nullptr;
    }

    bool destroyRelationshipToVertex(unsigned int id) {
      Vertex<T> *current = verticesHead;
      while (current != nullptr) {
        Edge<T> *connectedEdge = current->searchEdgeById(id);
        if (connectedEdge != nullptr) {
          current->deleteEdge(connectedEdge);
        }
        current = current->next;
      }
      return true;
    }

    bool destroyAllVertices() {
      if (!isEmpty()) {
        Vertex<T> *current = verticesHead;
        while (current != nullptr) {
          Vertex<T> *toDelete = current;
          current = current->next;
          delete toDelete;
        }
      }
      vertexCount = 0;
      return true;
    }

    bool deleteVertexByPtr(Vertex<T> *vertex) {
      if (vertex == nullptr) return false;
      if (vertex->edgeList != nullptr) destroyRelationshipToVertex(vertex->id);

      if (vertex->prev != nullptr) vertex->prev->next = vertex->next;
      if (vertex->next != nullptr) vertex->next->prev = vertex->prev;
      if (vertex == verticesHead) verticesHead = verticesHead->next;

      vertexCount--;
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
      Vertex<T> *current = verticesHead;
      while (current != nullptr) {
        cout << "[" << current->id << "] " << " -> ";
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

    bool saveToBin(string filePath) {
      ofstream outFile(filePath, ios::binary | ios::trunc);
      if (!outFile) {
        cerr << "Error opening file for writing" << endl;
        return false;
      }

      outFile.write(reinterpret_cast<char *>(&vertexCount), sizeof(vertexCount));

      Vertex<T> *current = verticesHead;
      while (current != nullptr) {
        outFile.write(reinterpret_cast<char *>(&current->id), sizeof(current->id));
        outFile.write(reinterpret_cast<char *>(&current->degree), sizeof(current->degree));
        // current->data.serialize(outFile);
        outFile.write(reinterpret_cast<char *>(&current->data), sizeof(current->data));
        current = current->next;
      }

      current = verticesHead;
      while (current != nullptr) {
        Edge<T> *currentEdge = current->edgeList;
        while (currentEdge != nullptr) {
          unsigned int refId = currentEdge->vertexRef->id;
          outFile.write(reinterpret_cast<char *>(&refId), sizeof(refId));
          currentEdge = currentEdge->next;
        }
        current = current->next;
      }

      outFile.close();
      return true;
    }

    bool loadFromBin(string filePath) {
      ifstream inFile(filePath, ios::binary);
      if (!inFile) {
        cerr << "Error opening file for reading" << endl;
        return false;
      }

      destroyAllVertices();

      int loadedVertexCount;
      inFile.read(reinterpret_cast<char *>(&loadedVertexCount), sizeof(loadedVertexCount));

      for (int i = 0; i < loadedVertexCount; i++) {
        unsigned int id;
        int degree;
        T data;

        inFile.read(reinterpret_cast<char *>(&id), sizeof(id));
        inFile.read(reinterpret_cast<char *>(&degree), sizeof(degree));
        // data.deserialize(inFile);
        inFile.read(reinterpret_cast<char *>(&data), sizeof(data));

        Vertex<T> *newVertex = new Vertex<T>(data, id, degree);
        addVertex(newVertex);
      }

      Vertex<T> *current = verticesHead;
      while (current != nullptr) {
        int loadedEdgeCount = current->degree;
        current->degree = 0;
        for (int i = 0; i < loadedEdgeCount; i++) {
          unsigned int refId;
          inFile.read(reinterpret_cast<char *>(&refId), sizeof(refId));
          addEdgeById(current->id, refId);
        }
        current = current->next;
      }

      inFile.close();
      return true;
    }
  };
}
