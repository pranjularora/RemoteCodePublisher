#pragma once
/////////////////////////////////////////////////////////////////////////////
// NoSqlDb.cpp - Test STub for NoSqlDb.h                                   //
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

#include "noSqlDB.h"
#include <iostream>
#include "strhelper.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../XmlDocument/XmlParser/XmlParser.h"
#include <fstream>
#include <time.h>

using namespace std;
using StrData = string;
using intData = int;
using Key = noSqlDB<StrData>::Key;
using Keys = noSqlDB<StrData>::Keys;
using namespace XmlProcessing;

using SPtr = std::shared_ptr<AbstractXmlElement>;


#ifdef TEST_NOSQLDB
int main()
{
	cout << "\n =================================\n";
	noSqlDB<StrData> db;
	Element<StrData> elem1;
	cout << "\n  Adding Data and saving it to the database\n\n";
	elem1.name = "pranjul";
	elem1.category = "Test1";
	// yyyy-mm-dd
	elem1.timeDate = "2017-02-03 04:19:33";
	elem1.data = "Elem1 data";
	db.save("pranjul", elem1);
	Element<StrData> elem2;
	elem2.name = "arora";
	elem2.category = "Test2";
	elem2.data = "Elem2 data";
	elem2.timeDate = "2017-02-01 04:19:33";
	elem2.children.push_back(elem2.name);
	db.save("key2", elem2);
	Keys keys = db.keys();
	for (Key key : keys)
	{
		cout << "\n  " << key << ":";
		cout << db.value(key).show();
	}
	cout << "\n ------------------------------------------\n";

	db.deleteElem("pranjul");
	db.editElem("key2", "Edited", "");
	cout << "\n  After Deletion and editing category of key2 from NoSqlDb<string>";
	cout << "\n ------------------------------------------\n";
	cout << "\n  size of db = " << db.count() << "\n";
	Keys keys1 = db.keys();
	for (Key key : keys1)
	{
		cout << "\n  " << key << ":";
		cout << db.value(key).show();
	}
}

#endif



