#pragma once
/////////////////////////////////////////////////////////////////////////////
// FileB.h - Testing File B                                                //
//																		   //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright � Pranjul Arora, 2017                                         //
// All rights granted provided that this notice is retained                //	
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2015                             //
// Platform:    Mac Book Pro, Core i5, MAC OS                              //
// Application: Project 2, CSE687 - Object Oriented Design                 //
// Author:      Pranjul Arora, Syracuse University                         //
//              (315) 949 9061, parora01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "FileC.h"


class FileB {
public:
	void showB();
	enum type { One, Two, Three };
private:
	TestC::FileC c;

};

void FileB::showB() {
	std::cout << "\nInside File B show Function\n";
	c.showC();
}


