#include <iostream>
#include <vector>

using namespace std;

// vertex contains an int value which corresponds to its position in the vector and
// a vector of edges (pointers to other vertices in the graph)
struct vertex {
	vector <vertex*> edges;
	int position = -1;
};

class Iterator {

	// missing vertex will be used by the findMissingVertex member function.
	// linksAdded wil be used
	int missingVertex = -1;
	
public:
	vector <bool> verticesChecked;

	// constructor for iterator takes in a size which should correspond to a given vector
	// size.  it will create a mirror vector of bool flags for each position of the vector
	// to be used when traversing the graph.
	Iterator(int size) {
		for (int i = 0; i < size; ++i) {
			verticesChecked.push_back(false);
		}
	}

	// resets all flags in a given vector to false.  This will take place in between
	// traversals to reset all flags to false prior to another check.
	void resetVector(vector<bool>& vectorToReset) {
		for (int i = 0; i < vectorToReset.size(); ++i){
			verticesChecked[i] = false;
		}
	}

	// sets the bool flag in the verticeChecked vector that corresponds to the current
	// vertex to true. looks through the vector containing the edges of the current vertex.
	// if the corresponding bool flag is set to true, that vertex has already been checked and
	// loop will continue to next edge iteration. if that vertex has not already been checked,
	// a recursive traverseGraph function call will be made on that vertex.  This will continue
	// until the iterator runs out of edges to check.
	void traverseGraph(vertex* vertexToCheck) {
		verticesChecked[vertexToCheck->position] = true;
		for (int i = 0; i < vertexToCheck->edges.size(); ++i) {
			if (verticesChecked[vertexToCheck->edges[i]->position]) {
				continue;
			}
			else {
				traverseGraph(vertexToCheck->edges[i]);
			}
		}
	}

	// returns the value found in missingVertex.
	int getMissingVertex() {
		return missingVertex;
	}

	// returns the first instance of false in the verticesChecked boolean vector.
	// This will represent a missing vertex from the traversal.
	int findMissingVertex() {
		missingVertex = -1;
		for (int i = 0; i < verticesChecked.size(); ++i) {
			if (verticesChecked[i] == false) {
				missingVertex = i;
				break;
			}
		}
		return missingVertex;
	}

	

};
class Graph {
	int linksAdded = 0;
	
public:
	
	vector <vertex> vertices;

	Graph(int size) {
		for (int i = 0; i < size; ++i) {
			vertex vertexToAdd;
			vertices.push_back(vertexToAdd);
			vertices[i].position = i;
		}
	}
	
	vertex* getRoot() {
		return &vertices[0];
	}

	// returns the size of the contained vector of vertices.
	int size() {
		return vertices.size();
	}

	// returns the value of linksAdded.
	int getLinksAdded(){
		return linksAdded;
	}

	// connects a vertex to the rest of the graph. takes the missingVertex value filled
	// from the findMissingVertex function and creates a edge from the [0] position to the
	// missing vertex and vice versa. this function will also
	// reset the verticesChecked vector in preperation for next iteration.
	void addMissingLink(Iterator& iterator1) {
		vertex* vertexPtr = &vertices[iterator1.findMissingVertex()];
		vertices[0].edges.push_back(vertexPtr);
		vertexPtr = &vertices[0];
		vertices[iterator1.findMissingVertex()].edges.push_back(vertexPtr);
		++linksAdded;
	}


	// adds an edge between two vertices (x and y). the edge is created by adding a pointer
	// to each vertex in the edges vector of the other.
	void addEdge(int x, int y) {
		vertex* vertexPtr = &vertices[y];
		vertices[x].edges.push_back(vertexPtr);
		vertexPtr = &vertices[x];
		vertices[y].edges.push_back(vertexPtr);
	}
};





int main() {

	// create two int variables and take user input for the number of vertices in the
	// graph and the number of edges given.
	int noOfVertices;
	int noOfEdges;
	cin >> noOfVertices;
	cin >> noOfEdges;

	// create a graph and pass the number of vertices.
	Graph graph1(noOfVertices);


	// take input and put into x and y variables.  This will get used in the addEdge
	// member function of the graph. x and y will represent the vertices to be connected.
	int x;
	int y;
	for (int i = 0; i < noOfEdges; ++i) {
		cin >> x;
		cin >> y;
		graph1.addEdge(x, y);
	}

	// create an iterator and pass in the size of the graph.
	Iterator iterator1(graph1.size());

	// traverse the graph trying to reach every node. call findMissingVertex to check
	// if there is an unconnected vertex, and if so call addMissingLink to add an
	// edge from the root vertex to the unconnected vertex, and then loop again
	// starting from the previously unconnected vertex.
	iterator1.traverseGraph(graph1.getRoot());
	while (iterator1.findMissingVertex() != -1) {
		graph1.addMissingLink(iterator1);
		vertex* vertexPtr = &graph1.vertices[iterator1.getMissingVertex()];
		iterator1.traverseGraph(vertexPtr);
	}

	 

	// ouput the number of links that were added to connect every vertex to the graph.
	cout <<  graph1.getLinksAdded();
}