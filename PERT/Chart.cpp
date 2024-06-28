#include "Chart.h"

/**
 * @brief Utility function used by `longestPath()` to perform the topological sort.
 *
 * @param v - The current vertex being visited.
 * @param visited - A vector of booleans indicating whether each vertex has been visited.
 * @param stack - The stack used to store the vertices in topological order.
 */
void Chart::topologicalSortUtil(int v, std::vector<bool>& visited, std::stack<int>& stack)
{
    visited[v] = true;

    for (auto& node : adj[v]) {
        if (!visited[node.getNumber()]) {
            topologicalSortUtil(node.getNumber(), visited, stack);
        }
    }

    stack.push(v);
}

/**
 * @brief Performs a classical topological sort on the graph and finds the longest path from the specified source vertex.
 *
 * @param s - The source vertex.
 */
void Chart::longestPath(int s) {
    std::stack<int> stack;
    std::vector<int> dist(vertices, INT_MIN);
    std::vector<bool> visited(vertices, false);

    for (int i = 0; i < vertices; i++) {
        if (!visited[i]) {
            topologicalSortUtil(i, visited, stack);
        }
    }

    dist[s] = 0;

    while (!stack.empty()) {
        int u = stack.top();
        stack.pop();

        if (dist[u] != INT_MIN) {
            for (auto& node : adj[u]) {
                if (dist[node.getNumber()] < dist[u] + node.getWeight()) {
                    dist[node.getNumber()] = dist[u] + node.getWeight();
                }
            }
        }
    }

    std::cout << "Longest path from " << s << " to:\n";
    for (int i = 0; i < vertices; i++) {
        if (dist[i] == INT_MIN) {
            std::cout << "INF ";
        }
        else {
            std::cout << dist[i] << " ";
        }
    }
    std::cout << std::endl;
}

/**
 * @brief Constructs a new `Chart` object with the specified number of vertices.
 *
 * @param vertices - The number of vertices in the graph.
 */
Chart::Chart(int vertices)
{
	this->vertices = vertices;
	this->adj = new std::list<Node>[vertices];
}

/**
 * @brief Adds an edge to the graph with the specified source, destination, and weight.
 *
 * @param v - The source vertex.
 * @param w - The destination vertex.
 * @param weight - The weight of the edge.
 */
void Chart::addEdge(int v, int w, int weight)
{
	Node node(w, weight);
	adj[v].push_back(node);
	std::cout << "[LOG] Edge added: " << v << " -> " << w << " (" << weight << ")\n";
}

/**
 * @brief Reads the graph data from a file and adds the edges to the graph.
 *
 * @param fileName - The name of the file containing the graph data. If not provided, the default file name is "input.txt".
 */
void Chart::readFromFile(std::string fileName)
{
	std::ifstream file(fileName);
	if (file.is_open())
	{
		int node1, node2, weight;

		while (file >> node1 >> node2 >> weight)
		{
			addEdge(node1, node2, weight);
		}

		file.close();
	} else {
		std::cout << "[ERROR] Unable to open file " << fileName << "\nPlease check the file location.\n";
	}
}

/**
 * @brief Performs a modified versio of longest path on the DAG.
 *
 * @param s - The source vertex.
 */
void Chart::pertChart(int s)
{
    /*  I used https ://cs.boisestate.edu/~jhyeh/cs521/cs521_fall06/lab3.pdf
        as a reference for this implementation of the PERT chart.
        I also included it in the documentation.
    */

    std::stack<int> stack;
    std::vector<int> earliestStart(this->vertices, INT_MIN);
    std::vector<int> earliestFinish(this->vertices, INT_MIN);
    std::vector<int> latestStart(this->vertices, INT_MAX);
    std::vector<int> latestFinish(this->vertices, INT_MAX);
    std::vector<bool> visited(this->vertices, false);

    // Not a clever way to store the topological order, but it works.
    // I don't need to run the topological sort again, so I'm just storing the order in a vector.
    std::vector<int> topOrder;

    for (int i = 0; i < this->vertices; i++) {
        if (!visited[i]) {
			topologicalSortUtil(i, visited, stack);
		}
	}

    while (!stack.empty()) {
        topOrder.push_back(stack.top());
        stack.pop();
    }

    earliestStart[s] = 0;
    earliestFinish[s] = 0;

    for (int u : topOrder) {
        if (earliestStart[u] != INT_MIN) {
            for (auto& node : adj[u]) {
                // Relaxation step for updating the earliest start and finish times
                // The earliest finish time of a node is the maximum of the earliest start time of its predecessors
                if (earliestStart[node.getNumber()] < earliestStart[u] + node.getWeight()) {
                    earliestStart[node.getNumber()] = earliestStart[u] + node.getWeight();
                    earliestFinish[node.getNumber()] = earliestStart[node.getNumber()];
                }
            }
        }
    }

    latestFinish[topOrder.back()] = earliestFinish[topOrder.back()];
    latestStart[topOrder.back()] = earliestFinish[topOrder.back()];

    for (auto it = topOrder.rbegin(); it != topOrder.rend(); ++it) {
        int u = *it;
        if (latestFinish[u] != INT_MAX) {
            for (auto& node : adj[u]) {
                // Relaxation step for updating the latest start and finish times
                // The latest start time of a node is the minimum of the latest finish time of its successors
                if (latestStart[u] > latestFinish[node.getNumber()] - node.getWeight()) {
                    latestStart[u] = latestFinish[node.getNumber()] - node.getWeight();
                    latestFinish[u] = latestStart[u];
                }
            }
        }
    }

    std::cout << "Earliest Start Times: ";
    for (int i = 0; i < vertices; i++) {
        if (earliestStart[i] == INT_MIN) {
			std::cout << "INF ";
        } else {
            std::cout << earliestStart[i] << " ";
        }
    }
    std::cout << "\n";

    std::cout << "Earliest Finish Times: ";
    for (int i = 0; i < vertices; i++) {
        if (earliestFinish[i] == INT_MIN) {
            std::cout << "INF ";
        } else {
            std::cout << earliestFinish[i] << " ";
        }
    }
    std::cout << "\n";

    std::cout << "Critical Path: ";
    for (int i = 0; i < vertices; i++) {
        if (earliestStart[i] == latestStart[i]) {
            std::cout << i << " ";
        }
    }

    std::cout << "\n";
}
