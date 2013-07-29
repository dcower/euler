#include <queue>
#include <set>
#include <map>
#include <list>
#include <iterator>
#include <functional>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>


// A vertex in a graph.
class Vertex
{
public:

	Vertex(unsigned int id_) : id(id_)
	{
	}

	bool operator<(const Vertex &other) const
	{
	    return id < other.id;
	}

private:

	unsigned int id;
};


// A directed edge in a graph.
class Edge
{
public:

	Edge(Vertex vertexA_, Vertex vertexB_, int cost_) : vertexA(vertexA_), vertexB(vertexB_), cost(cost_)
	{
	}

	int getCost() const
	{
		return cost;
	}

	const Vertex &getVertexA() const
	{
		return vertexA;
	}

	const Vertex &getVertexB() const
	{
		return vertexB;
	}

private:

	Vertex vertexA, vertexB;
	int cost;
};


// Comparator for comparing const Edge * by their costs, in reverse (greater than)
struct EdgePointerGT : public std::binary_function<Edge*, Edge*, bool>
{
	bool operator()(const Edge *&left, const Edge *&right) const
	{
		return left->getCost() > right->getCost();
	}
};


// A directed graph implemented using adjacency lists.
class Graph
{
public:

	// Insert the given edge into the graph.
	void insertEdge(const Edge &edge)
	{
		vertices.insert(edge.getVertexA());
		vertices.insert(edge.getVertexB());

		edges[edge.getVertexA()].push_back(edge);
	}

	// Does this graph contain the given vertex?
	bool hasVertex(const Vertex &vertex) const
	{
		return vertices.find(vertex) != vertices.end();
	}

	// Calculate the total cost of all edges in this graph.
	int calculateCost() const
	{
		int sum = 0;

	for (const auto &i : edges)
			for (const auto &j : i.second)
				sum += j.getCost();

		return sum;
	}

	// Create a minimum spanning tree from this graph, storing it in the given graph.
	// It is assumed that this graph is directed and connected.
	// Prim's algorithm is used.
	void createMinimumSpanningTree(Graph &result)
	{
		if (edges.empty())
			return;

		std::priority_queue<const Edge*, std::deque<const Edge*>, EdgePointerGT> queue;

		// we assume the graph is connected, so we start the process with the first vertex's edges
		for (const auto &edge : edges[*vertices.begin()])
			queue.push(&edge);

	// Proceed until we run out of edges to expand or the MST is complete
		while (!queue.empty() && result.vertices.size() < this->vertices.size())
		{
			const Edge *edge = queue.top();
			queue.pop();

			if (!result.hasVertex(edge->getVertexB()))
			{
				result.insertEdge(*edge);
				result.insertEdge(Edge(edge->getVertexB(), edge->getVertexA(), edge->getCost()));

		for (const auto &i : edges[edge->getVertexB()])
					queue.push(&i);
			}
		}
	}

private:

	std::map<Vertex, std::list<Edge> > edges;
	std::set<Vertex> vertices;
};


// Reads in the comma-separated rows (lines) of an adjacency matrix into the given graph.
// Currently, graph filename is hardcoded for P.E. 107's given file.
void readGraph(Graph &graph)
{
	std::ifstream fin("network.txt");
	std::string line;

	for (unsigned int row = 0; std::getline(fin, line); ++row)
	{
		std::stringstream lineStream(line);
		std::string element;

		for (unsigned int column = 0; std::getline(lineStream, element, ','); ++column)
		{
			if (element.compare("-") != 0)
			{
				int cost = std::atoi(element.c_str());

				graph.insertEdge(Edge(row, column, cost));
			}
		}
	}
}


int main()
{
	Graph graph;
	Graph mst;
	readGraph(graph);

	graph.createMinimumSpanningTree(mst);

	std::cout << "Cost of Incoming Graph: " << graph.calculateCost()/2 << std::endl;
	std::cout << "Cost of Reduced Graph: " << mst.calculateCost()/2 << std::endl;
	std::cout << "Difference: " << (graph.calculateCost()-mst.calculateCost())/2 << std::endl;

	return 0;
}
