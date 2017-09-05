#pragma once
/////////////////////////////////////////////////////////////////////////////
// strongComponent.cpp - Implementation file for strongComponent.h         //
//																		   //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Pranjul Arora, 2017                                         //
// All rights granted provided that this notice is retained                //	
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2015                             //
// Platform:    Mac Book Pro, Core i5, MAC OS                              //
// Application: Project 2, CSE687 - Object Oriented Design                 //
// Author:      Pranjul Arora, Syracuse University                         //
//              (315) 949 9061, parora01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////

#include "strongComponent.h"
ComponentVector StrongComponents::retrieveDisplayVector()
{
	return componentVector;
}

void StrongComponents::CreateGraph(DependencyTable& depTable)
{

	ElementDep local_elem;
	std::vector<std::string> keys = depTable.keys();
	std::unordered_map<std::string, size_t> map_of_vertex;
	// creating a map with vertex value as key and vertex id as value.
	for (size_t i = 0; i < keys.size(); i++)
	{
		vertex v(keys.at(i));
		grph.addVertex(v);
		map_of_vertex[keys.at(i)] = v.id();
	}

	for (size_t i = 0; i < keys.size(); i++)
	{
		local_elem = depTable.value(keys.at(i));

		// calculating index from the value in the above created hashMap and then using 
		// typename Vertex<V, E>& Graph<V, E>::operator[](size_t i) { return adj[i]; } to get value of vertex.
		size_t parent_index = map_of_vertex.at(keys.at(i));
		vertex parent = grph[parent_index];
		//	std::cout << "\nvalue of corressponding address: " << parent << "  -->  " << parent.value().c_str();
		for (auto setVal : local_elem.dependentFiles)
		{
			size_t child_index = map_of_vertex.at(setVal);
			vertex child = grph[child_index];
			grph.addEdge("", parent, child);
		}
	}
	//	display::show(grph); // display graph
	std::cout << "\n";
}

void StrongComponents::SCC()
{
	int *disc = new int[grph.size()];
	int *low = new int[grph.size()];
	bool *stackMember = new bool[grph.size()];
	std::stack<vertex> *st = new std::stack<vertex>();

	// Initialize disc and low, and stackMember arrays
	for (size_t i = 0; i < grph.size(); i++)
	{
		disc[i] = INT_MIN;
		low[i] = INT_MIN;
		stackMember[i] = false;
	}

	// Call the recursive helper function to find strongly
	// connected components in DFS tree with vertex 'i'

	int i = 0;
	for (auto vertex : grph.getAdjacencyList())
	{
		if (disc[i] == INT_MIN)
			SCCUtil(vertex, disc, low, st, stackMember);
		i++;
	}
}

void StrongComponents::SCCUtil(vertex u, int disc[], int low[], std::stack<vertex> *st, bool stackMember[])
{
	// A static variable is used for simplicity, we can avoid use
	// of static variable by passing a pointer.
	static int time = 0;

	// Initialize discovery time and low value
	disc[u.id()] = low[u.id()] = ++time;
	st->push(u);
	stackMember[u.id()] = true;

	// Go through all vertices adjacent to this
	for (std::pair<int, std::string> edge : u.getEdges())
	{
		int v = edge.first;
		if (disc[v] == INT_MIN)
		{
			SCCUtil(grph[v], disc, low, st, stackMember);
			low[u.id()] = (std::min)(low[u.id()], low[v]);
		}
		else if (stackMember[v] == true) {
			low[u.id()] = (std::min)(low[u.id()], disc[v]);
		}
	}
	int w = 0;

	if (low[u.id()] == disc[u.id()])
	{
		std::vector<vertex> localVector;
		while (st->top().id() != u.id())
		{
			w = (int)st->top().id();
			localVector.push_back(grph[w]);
			//std::cout << grph[w].value().c_str() << " ";
			stackMember[w] = false;
			st->pop();
		}
		w = (int)st->top().id();

		//std::cout << grph[w].value().c_str() << " ";
		//std::cout << "\n";
		localVector.push_back(grph[w]);
		stackMember[w] = false;
		st->pop();
		componentVector.push_back(localVector);
	}
}

#ifdef StrongComponents

int main() {
	using Key = std::string; // TypeName is the key
	using Keys = std::vector<Key>;
	// Test Stub
	std::cout << "\n =================================\n";
	ElementDep elem;
	DependencyTable dep;
	std::cout << "\n  Adding Data \n\n";
	elem.filename = "File A";
	elem.dependentFiles.insert({ "FileB","FileC" });


	Keys keys = dep.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		dep.save(key, elem);

	}
	StrongComponents strngCmpnts;
	strngCmpnts.CreateGraph(dep);
	strngCmpnts.SCC();


	std::cout << "\n ------------------------------------------\n";

}
#endif // StrongComponents
