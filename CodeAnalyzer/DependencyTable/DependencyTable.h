#pragma once
/////////////////////////////////////////////////////////////////////////////
// DependencyTable.h - Represents a Dependency Table                       //
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
* This package is for Dependency Table that creates dependency table based on the
* ananlysis done.
*
* Public Interface:
* -----------------
* Keys keys();
*  - function that retrieve keys stored in the Dependency Table map
*
* ElementDep value(Key);
*  - function that retrieve value(Element) corresponding to a key
*
* bool save(Key key, ElementDep elem);
*  - function that saves the Dependency Table.
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
#include <unordered_set>
#include <unordered_map>
#include "../CppProperties/CppProperties.h"


class ElementDep {
public:
	using FileName = std::string;
	using DependentFiles = std::unordered_set<std::string>;

	DependentFiles dependentFiles;
	Property<FileName> filename;
};



class DependencyTable {
public:
	using Key = std::string; // TypeName is the key
	using Keys = std::vector<Key>;
	Keys keys();
	ElementDep value(Key);
	bool save(Key key, ElementDep elem);

private:
	using Item = std::pair<Key, ElementDep>;
	std::unordered_map<Key, ElementDep> depMap;
};

