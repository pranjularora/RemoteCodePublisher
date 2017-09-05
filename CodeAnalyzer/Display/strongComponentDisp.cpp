/////////////////////////////////////////////////////////////////////////////
// strongTableDisp.cpp - Implementation file for strongComponentDisp.h     //
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

#include "strongComponentDisp.h"

void StrongComponentDisplay::displayStrongComponents(StrongComponents& strngComp)
{
	strongComponent = strngComp;
	componentVector = strongComponent.retrieveDisplayVector();
	std::cout << "\n==================================================";
	std::cout << "\n\t\t  Strong Components :\n";
	std::cout << "==================================================\n";

	int count = 1;
	for (auto component : componentVector)
	{
		std::cout << "\nStrong Component: " << count;
		int i = 0;
		for (auto vrtx : component)
		{
			if (i != 0)
				std::cout << "\n                   ";
			std::cout << "    " << std::setw(15) << vrtx.value().c_str();
			i++;
		}
		std::cout << "\n----------------------------------------------------------------------------------------------------\n";
		count++;

	}
}
// change 
#ifdef StrongComponentdisp
int main() {
	using Key = std::string; // TypeName is the key
	using Keys = std::vector<Key>;
	// Test Stub
	std::cout << "\n =================================\n";
	ElementDep elem;
	DependencyTable dep;
	StrongComponentDisplay sc;
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
	sc.displayStrongComponents();
}
#endif // StrongComponentdisp
