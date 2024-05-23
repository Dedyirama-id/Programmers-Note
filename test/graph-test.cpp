#include <cassert>
#include <iostream>
#include "../src/styles/test_styles.h"
#include "../src/class/graph.h"

using namespace gr;
using namespace test;

const string filePath = "data/graph.bin";

struct ForGraph {
  string name;
  string type;
  string description;
  string example;
};


void testAddVertex() {
  Graph<int> graph;
  assert(graph.isEmpty());

  graph.addVertex(100, 1);
  assert(graph.getVerticesTail() != nullptr);
  assert(graph.getVerticesTail()->id == 100);
  assert(graph.vertexCount == 1);

  graph.addVertex(200, 1);
  assert(graph.getVerticesTail()->id == 200);
  assert(graph.vertexCount == 2);

  printSuccess("testAddVertex passed!");
}

void testDeleteVertex() {
  Graph<int> graph;
  graph.addVertex(100, 1);
  graph.addVertex(200, 2);

  assert(graph.deleteVertexById(100));
  assert(graph.vertexCount == 1);
  assert(graph.searchById(100) == nullptr);

  assert(graph.deleteVertexById(200));
  assert(graph.vertexCount == 0);
  assert(graph.searchById(200) == nullptr);

  printSuccess("testDeleteVertex passed!");
}

void testAddEdge() {
  Graph<int> graph;
  graph.addVertex(100, 1);
  graph.addVertex(200, 2);
  graph.addVertex(300, 3);

  assert(graph.addEdgeById(100, 200));
  assert(graph.addEdgeById(100, 300));
  assert(graph.addEdgeById(200, 300));

  Vertex<int> *vertex1 = graph.searchById(100);
  assert(vertex1->edgeList != nullptr);
  assert(vertex1->degree == 2);

  Vertex<int> *vertex2 = graph.searchById(200);
  assert(vertex2->edgeList != nullptr);
  assert(vertex2->degree == 1);

  printSuccess("testAddEdge passed!");
}

void testDeleteEdge() {
  Graph<int> graph;
  graph.addVertex(100, 1);
  graph.addVertex(200, 2);
  graph.addVertex(300, 3);

  graph.addEdgeById(100, 200);
  graph.addEdgeById(100, 300);
  graph.addEdgeById(200, 300);

  Vertex<int> *vertex1 = graph.searchById(100);
  Edge<int> *edge1 = vertex1->searchEdgeById(200);
  assert(edge1 != nullptr);

  assert(vertex1->deleteEdge(edge1));
  assert(vertex1->degree == 1);
  assert(vertex1->searchEdgeById(200) == nullptr);

  printSuccess("testDeleteEdge passed!");
}

void testSaveAndLoadGraph() {
  Graph<ForGraph> graph;
  graph.addVertex(100, ForGraph{ "1", "Vertex1", "Description1", "Example1" });
  graph.addVertex(200, ForGraph{ "2", "Vertex2", "Description2", "Example2" });
  graph.addVertex(300, ForGraph{ "3", "Vertex3", "Description3", "Example3" });

  graph.addEdgeById(100, 200);
  graph.addEdgeById(100, 300);
  graph.addEdgeById(200, 300);

  assert(graph.saveToBin(filePath));

  Graph<ForGraph> loadedGraph;
  assert(loadedGraph.loadFromBin(filePath));
  assert(loadedGraph.vertexCount == graph.vertexCount);

  Vertex<ForGraph> *vertex1 = loadedGraph.searchById(100);
  Vertex<ForGraph> *vertex2 = loadedGraph.searchById(200);
  Vertex<ForGraph> *vertex3 = loadedGraph.searchById(300);

  assert(vertex1 != nullptr && vertex1->data.name == "1" && vertex1->degree == 2);
  assert(vertex2 != nullptr && vertex2->data.name == "2" && vertex2->degree == 1);
  assert(vertex3 != nullptr && vertex3->data.name == "3" && vertex3->degree == 0);

  assert(vertex1->searchEdgeById(200) != nullptr);
  assert(vertex1->searchEdgeById(300) != nullptr);
  assert(vertex2->searchEdgeById(300) != nullptr);

  printSuccess("testSaveAndLoadGraph passed!");
}


int main() {
  cout << "Running tests..." << endl;

  testAddVertex();
  testDeleteVertex();
  testAddEdge();
  testDeleteEdge();
  testSaveAndLoadGraph();

  cout << "All tests passed!" << endl;
  return 0;
}
