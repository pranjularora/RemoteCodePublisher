#pragma once
/////////////////////////////////////////////////////////////////////////////
// FileA.h - Testing File A                                                //
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
#include "FileB.h"
#include "FileC.h"
#include <iostream>


struct  x
{
	{
		
	}
};
void abc() {}
namespace TestA {
	void def() {	}
	class FileA {
	public:
		void showA();

	private:
		FileB fileB;
		TestC::FileA a;

	};

	void FileA::showA() {
		std::cout << "\nInside Show Function of FileA\n";
		fileB.showB();

	}
}
