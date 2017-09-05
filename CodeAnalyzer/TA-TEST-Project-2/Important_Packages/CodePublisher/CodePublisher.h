#pragma once
/////////////////////////////////////////////////////////////////////////////
// CodePublisher.h - Represents a Dependency Table                         //
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
* This package takes all the cpp and .h files specified on the command line and pushlishes them to web page
* 
*
* Public Interface:
* -----------------
* void doAnalysis(int argc, char* argv[], DependencyTable&, TypeTable&, CodeAnalysis::CodeAnalysisExecutive&);
*  - This function calls the Project#2 functionality and creates type table and dependency packages.
*
* void displayPublisher(std::vector<std::string>, int argc, char* argv[], std::string);
*  - This function helps displaying publisher's functionality through an index page or a specific file given as 
*    command line argument. 
*    Private Functions used- processCommandLineArgs()- helps processing command line argument.
*							 
*
* std::string getWebPageRepo(int argc, char* argv[])
*  - Process command line and check if user has given any location where he wants to create the repository.
*
* int publishFiles(Files, Files&, File, DependencyTable&, TypeTable&)
*  - using this function the codePublisher do all the publishing.
*    Private Functions used- createBody()- creates html body; 
*							 checkDiv() - helps implementing hiding and unhiding functionality.
*
* void displayRequirements();
*  - Helps displaying all the Project#3 requirement.
*
*  void displayPattern(int);
* - For displaying style
*
* Required Files:
* ---------------
* CppProperties.h
*
* Build Process:
* --------------
* devenv CodeAnalyzer.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 4th April 2017
* - first release
*
*/


#include <iostream>
#include <fstream>
#include "../FileMgr/IFileMgr.h"
#include "../FileSystem/FileSystem.h"
#include "../Analyzer/DepAnal.h"
#include "../Analyzer/Executive.h"
#include "../DependencyTable/DependencyTable.h"
#include "../Display/DependencyTableDisp.h"
#include "../Display/TypeTableDisp.h"



using Dirs = std::vector<std::string>;
using Files = std::vector<std::string>;
using File = std::string;
using Path = std::string;

class CodePublisher {
public:
	void doAnalysis(int argc, char* argv[], DependencyTable&, TypeTable&, CodeAnalysis::CodeAnalysisExecutive&);
	void displayPublisher(std::vector<std::string>, int argc, char* argv[], std::string);
	std::string getWebPageRepo(int argc, char* argv[]);
	int publishFiles(Files, Files&, File, DependencyTable&, TypeTable&);
	void displayRequirements();
	void displayPattern(int);

private:
	void createBody(std::ofstream&, std::string, std::string, DependencyTable, std::ifstream&);
	std::vector<std::string> splitAndReplace(std::string);
	std::string processCommandLineArgs(int argc, char* argv[], std::string);
	int checkFilesState(std::ifstream&, std::ifstream&, std::ofstream&);
	void replaceGTLTchars(std::string&);
	void checkDiv(bool&, std::string&, TypeTable&);
	void insertDivClassTag(bool&, std::string&, int&, std::ofstream&, int&);
	
};