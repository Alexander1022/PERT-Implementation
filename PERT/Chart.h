#pragma once
#include <iostream>
#include <list>
#include <stack>
#include <fstream>
#include <string>
#include <vector>
class Chart
{
private:
	struct Node 
	{
		int number;
		int weight;

		Node(int number, int weight) : number(number), weight(weight) {}
		const int getNumber() const { return number; }
		const int getWeight() const { return weight; }
	};

	int vertices;
	std::list<Node> *adj;
	void topologicalSortUtil(int v, std::vector<bool>& visited, std::stack<int>& stack);
public: 
	Chart(int vertices);
	void longestPath(int s);
	void addEdge(int v, int w, int weight);

	// In default case, the file name is "inpuit.txt"
	void readFromFile(std::string fileName = "input.txt");
	void pertChart(int s);
};

