#pragma once
/////////////////////////////////////////////////////////////////////////////
// FileC.h - Testing File C                                                //
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
#include "FileD.h"
#include "FileA.h"

void xyz() {}
namespace TestC {
	class FileA {};
	void ygt() {}
	class FileC {
	public:
		void showC();
	private:
		TestA::FileA a;
		FileD d;
	};

	void FileC::showC()
	{
		a.showA();
		d.showD();
		std::cout << "\nInside show function of FileC\n";
	}

}
