#pragma once
/////////////////////////////////////////////////////////////////////////////
// strongComponentDisp.h - Display Strong Components                       //
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
* void displayStrongComponents(StrongComponents&);
*  - function that displays strong components
*
*
*
* Required Files:
* ---------------
* strongComponent.h
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
#include "../StrongComponents/strongComponent.h"

typedef Vertex<std::string, std::string> vertex;

using ComponentVector = std::vector<std::vector<vertex>>;

class StrongComponentDisplay
{
public:
	void displayStrongComponents(StrongComponents&); //display Strong Components
private:
	StrongComponents strongComponent;
	ComponentVector componentVector; // vector of vertex
};

