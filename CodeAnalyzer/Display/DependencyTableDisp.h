#pragma once
/////////////////////////////////////////////////////////////////////////////
// DependencyTableDisp.h - Display Dependency Table                        //
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
* void displayDependencyTable(DependencyTable&);
*  - function that displays Dependency Table
*
*
*
* Required Files:
* ---------------
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

#include "../DependencyTable/DependencyTable.h"

using Key = std::string;
using Keys = std::vector<Key>;
using DependentFiles = std::unordered_set<std::string>;

class DependencyTableDisp
{
public:
	void displayDependencyTable(DependencyTable&); //display dependencyTable
private:
	std::string show(ElementDep&);
	DependentFiles dependentFiles; // unordered_set of dependent files

};


