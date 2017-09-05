#pragma once
/////////////////////////////////////////////////////////////////////////////
// Persist.cpp - Test STub for Persist.h                                   //
//																		   //
// ver 2.0                                                                 //
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
#include "Persist.h"


#ifdef Persist
int main()
{
	Persist<std::string> persist;
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
	persist.getData(dep);
	persist.saveToNoSqlDB();
	persist.displayNoSQLDB();
	persist.persist("testStubXML.xml");
}
#endif // Persist
