/////////////////////////////////////////////////////////////////////////////
// DependencyTableDisp.cpp - Implementation file for DependencyTableDisp.h //
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


#include "DependencyTableDisp.h"
void DependencyTableDisp::displayDependencyTable(DependencyTable& dependencyTable)
{
	std::cout << "\n\n\n==================================================";
	std::cout << "\n\t\t  Displaying Dependency Table :\n";
	std::cout << "==================================================\n";

	Keys depTablKey = dependencyTable.keys();
	for (Key key : depTablKey)
	{
		std::cout << "\nFile Name: " << "\t\t\t" << std::setw(8) << key;
		ElementDep dep = dependencyTable.value(key);
		std::cout << "\n\nDependencies:\t\t\t" << show(dep);
	}
}

std::string DependencyTableDisp::show(ElementDep& elemDep) {
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);

	int count = 0;
	for (auto name : elemDep.dependentFiles)
	{

		if (count != 0)
			out << "\t\t\t\t" << std::setw(15) << name << "\n";
		else
			out << name << "\n";
		count++;

	}
	out << "\n----------------------------------------------------------------------------------------------------\n";
	return out.str();
}


#ifdef DependencyTableDisp
int main() {
	using Key = std::string; // TypeName is the key
	using Keys = std::vector<Key>;
	DependencyTableDisp depTabl;
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
	depTabl.displayDependencyTable(dep);
	std::cout << "\n ------------------------------------------\n";


}
#endif // DependencyTableDisp
