#pragma once
/////////////////////////////////////////////////////////////////////////////
// TypeTableDisp.h - Display Type Table                                    //
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
* void displayTypeTable_Tab(TypeTable&);
*  - function that displays type table in tabular form
*
* void displayTypeTable(TypeTable&);
*  - function that displays type table in non- tabular form
*
*
* Required Files:
* ---------------
* TypeTable.h
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
#include "../TypeTable/TypeTable.h"

using Key = std::string;
using Keys = std::vector<Key>;

class TypeTableDisp
{
public:
	void displayTypeTable(TypeTable&);
	void displayTypeTable_Tab(TypeTable&);
private:
	std::string show(Element&);// show ouput in non-tabular form
	std::string show_Tab(Element&); // show ouput in tabular form

};