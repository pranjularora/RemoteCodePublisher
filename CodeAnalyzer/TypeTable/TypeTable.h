#pragma once
/////////////////////////////////////////////////////////////////////////////
// TypeTable.h - Represents a Type Table                                   //
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
/*
* Module Operations:
* -------------------
* This package is for Type Table that creates Type table based on the
* ananlysis done.
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
* Keys keys();
*  - function that retrieve keys stored in the Type Table map
*
* std::vector<Element> value(Key);
*  - function that retrieve value(Element) corresponding to a key
*
* bool save(Key key, Element elem);
*  - function that saves the Type Table.
*
* bool containsKey(Key);
*  - function that check whether a key is present in Type map.
*
* Required Files:
* ---------------
* CppProperties.h
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

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include "../CppProperties/CppProperties.h"


class Element {
public:
	using Type = std::string;
	using Path = std::string;

	using Package = std::string;
	using Namespace = std::string;

	using Pair_of_Dep = std::pair<Package, Namespace>;
	Property<Type> type;
	Pair_of_Dep pair_of_dep;
	Property<Path> path;
};





class TypeTable {
public:
	using Key = std::string; // TypeName is the key
	using Keys = std::vector<Key>;
	Keys keys(); // return keys
	std::vector<Element> value(Key); // return value corresponding to a key 
	bool save(Key key, Element elem); // save
	bool containsKey(Key);
private:
	using Item = std::vector<std::pair<Key, Element>>;
	std::unordered_map<Key, std::vector<Element>> typeMap;
};

