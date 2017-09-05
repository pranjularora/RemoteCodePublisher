#pragma once
/////////////////////////////////////////////////////////////////////////////
// stongComponent.h - Interacts with the Graph class to find Stong         //
//			          Components using Tarjan's Algorithm                  //
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
/*
* Module Operations:
* -------------------
* This package is for Strong Components that creates Storng Components analysis based on the
* graph present using Tarjan's Algorithm
*
*
/*
* Module Operations:
* -------------------
* This package is for Type Table that creates Type table based on the
* ananlysis done.
*
* Public Interface:
* -----------------
* void CreateGraph(DependencyTable&);
*  - creates graph
*
* void SCC();
*  - prints strongly connected components
*
* ComponentVector retrieveDisplayVector();
*  - retrieve vertex vector, a data memeber
*
*
* Required Files:
* ---------------
* Graph.h
* DependencyTable.h
*
* Build Process:
* --------------
* devenv CodeAnalyzer.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 7th March 2017
* - first release
*
*/


#undef min
#include <iostream>
#include <algorithm>
#include <list>
#include <stack>
#include <vector>
#include "../Graph/Graph.h"
#include "../DependencyTable/DependencyTable.h"
using namespace GraphLib;


typedef Graph<std::string, std::string> graph;
typedef Vertex<std::string, std::string> vertex;
typedef Display<std::string, std::string> display;

using Key = std::string; // TypeName is the key
using Keys = std::vector<Key>;
using ComponentVector = std::vector<std::vector<vertex>>;
class StrongComponents {

public:

	void CreateGraph(DependencyTable&);
	void SCC();    // prints strongly connected components
	ComponentVector retrieveDisplayVector();
private:
	graph grph;
	void SCCUtil(vertex u, int disc[], int low[], std::stack<vertex> *st, bool stackMember[]);
	ComponentVector componentVector;
};





