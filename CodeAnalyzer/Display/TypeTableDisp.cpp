/////////////////////////////////////////////////////////////////////////////
// TypeTableDisp.cpp - Implementation file for TypeTableDisp.h             //
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

#include "TypeTableDisp.h"

using vecElements = std::vector<Element>;
void TypeTableDisp::displayTypeTable(TypeTable& typeTable)
{
	std::cout << "\n\n\n========================================================================================================";
	std::cout << "\n\t\t  Displaying Type Table :--> As sometimes Data goes out of screen in Tabular Form\n";
	std::cout << "========================================================================================================\n";

	Keys typTablKey = typeTable.keys();

	for (Key key : typTablKey)
	{
		for (auto val : typeTable.value(key))
		{
			std::cout << std::left << "\n    " << std::setw(10) << "TYPE NAME" << " : " << key;
			std::cout << show(val);
		}

		std::cout << "----------------------------------------------------------------------------------------------------\n";
	}
}

void TypeTableDisp::displayTypeTable_Tab(TypeTable& typeTable)
{
	std::cout << "\n\n\n==================================================";
	std::cout << "\n\t\t  Displaying Type Table [Tabular Form] :\n";
	std::cout << "==================================================\n";
	Keys typTablKey = typeTable.keys();
	std::cout.setf(std::ios::adjustfield, std::ios::right);
	std::cout << "\n    " << std::setw(16) << "TYPE NAME" << std::setw(16) << "TYPE" << std::setw(16)
		<< "    " << std::setw(16)
		<< "NAMESPACE" << std::setw(16) << "    " << std::setw(16) << "PACKAGE" << std::setw(16)
		<< "    " << std::setw(16) << "Path";

	std::cout.setf(std::ios::adjustfield, std::ios::right);
	std::cout << "\n    " << std::setw(16) << "---------" << std::setw(16) << "---------" << std::setw(16)
		<< "    " << std::setw(16)
		<< "---------" << std::setw(16) << "    " << std::setw(16) << "---------" << std::setw(16)
		<< "    " << std::setw(16) << "---------";

	for (Key key : typTablKey)
	{
		for (auto val : typeTable.value(key))
		{
			std::cout << std::right << "\n    " << std::setw(16) << key << std::setw(16);
			std::cout << show_Tab(val);
		}

	}

}

std::string TypeTableDisp::show_Tab(Element& elem)
{
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::right);
	out << "    " << std::setw(16) << elem.type << std::setw(16);
	out << "    " << std::setw(16) << elem.pair_of_dep.first << std::setw(16)
		<< "    " << std::setw(16) << elem.pair_of_dep.second << std::setw(16);
	out << "    " << std::setw(16) << elem.path;

	return out.str();


}


std::string TypeTableDisp::show(Element& elem) {
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n    " << std::setw(10) << "TYPE" << " : " << elem.type
		<< "\n    " << std::setw(10) << "NAMESPACE" << " : " << elem.pair_of_dep.first
		<< "\n    " << std::setw(10) << "PACKAGE" << " : " << elem.pair_of_dep.second
		<< "\n    " << std::setw(10) << "PATH" << " : " << elem.path << "\n";

	return out.str();
}

#ifdef TypeTableDisp

int main() {
	using Key = std::string; // TypeName is the key
	using Keys = std::vector<Key>;
	// Test Stub
	std::cout << "\n =================================\n";
	TypeTableDisp type;
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
	type.displayTypeTable(dep);
	type.displayTypeTable_Tab(dep);

	std::cout << "\n ------------------------------------------\n";


}
#endif // TypeTableDisp
