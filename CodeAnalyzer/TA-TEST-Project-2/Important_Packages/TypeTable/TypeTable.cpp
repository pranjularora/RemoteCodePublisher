#pragma once
/////////////////////////////////////////////////////////////////////////////
// TypeTable.cpp - Implementation file for TypeTable.h                     //
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

#include "TypeTable.h"


// check if it contains a specific key
bool TypeTable::containsKey(TypeTable::Key key)
{
	if (typeMap.find(key) != typeMap.end())
		return true;
	return false;
}



std::vector<Element> TypeTable::value(TypeTable::Key key)
{
	if (typeMap.find(key) != typeMap.end())
	{
		return typeMap[key];
	}
	return std::vector<Element>();
}

// return all keys
TypeTable::Keys TypeTable::keys()
{
	Keys keys;
	for (auto item : typeMap)
	{
		keys.push_back(item.first);
	}
	return keys;
}


bool TypeTable::save(Key key, Element elem) {
	if (typeMap.find(key) != typeMap.end()) {
		std::vector<Element> localElem;
		localElem = typeMap[key];

		localElem.push_back(elem);
		typeMap[key].push_back(elem);
		return true;
	}
	typeMap[key].push_back(elem);
	return true;
}

#ifdef TypeTable
int main() {
	using Key = std::string; // TypeName is the key
	using Keys = std::vector<Key>;
	// Test Stub
	std::cout << "\n =================================\n";
	Element elem;
	TypeTable dep;
	std::cout << "\n  Adding Data \n\n";
	elem.path = "../File A";
	elem.type = "class";
	elem.pair_of_dep.first = "FileB";
	elem.pair_of_dep.second = "FileC";



	Keys keys = dep.keys();
	for (Key key : keys)
	{
		std::cout << "\n  " << key << ":";
		dep.save(key, elem);

	}
	std::cout << "\n ------------------------------------------\n";




}
#endif // TypeTable

