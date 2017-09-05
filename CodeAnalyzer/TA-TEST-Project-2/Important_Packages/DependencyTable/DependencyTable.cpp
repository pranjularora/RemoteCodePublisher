#pragma once
/////////////////////////////////////////////////////////////////////////////
// DependencyTable.cpp - Implementation file for DependencyTable.h         //
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

#include "DependencyTable.h"


ElementDep DependencyTable::value(DependencyTable::Key key)
{
	if (depMap.find(key) != depMap.end())
	{
		return depMap[key];
	}
	return ElementDep();
}

DependencyTable::Keys DependencyTable::keys()
{
	Keys keys;
	for (Item item : depMap)
	{
		keys.push_back(item.first);
	}
	return keys;
}

bool DependencyTable::save(Key key, ElementDep elem) {
	if (depMap.find(key) != depMap.end()) {
		return false;
	}
	depMap[key] = elem;
	return true;
}

#ifdef DependencyTable
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
	std::cout << "\n ------------------------------------------\n";




}
#endif // DependencyTable

