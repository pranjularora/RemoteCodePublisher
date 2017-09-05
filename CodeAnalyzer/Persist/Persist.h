/////////////////////////////////////////////////////////////////////////////
// Persist.h - Provide persistance to the Type Ananlysis, by stroing in    //
//			   NOSql DB and and creates an XML                             //
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
* This package is for creating and Persisting the data base using NOSqlDB
*
* Public Interface:
* -----------------

void saveToNoSqlDB();

void getDataStrongComp(StrongComponents&);
void saveToNoSqlDB_StrongComp();

void displayNoSQLDB();
std::string toXML();
void persist(std::string);

* void getData(DependencyTable&);
*  - function that gets the DependencyTable object
*
* void getDataStrongComp(StrongComponents&);
*  - function that gets the strong component object
*
* void displayNoSQLDB();
*  - function that display no Sql DB
*
* std::string toXML();
*  - function that convertes to XML
*
* void persist(std::string);
*  - function that saves to NoSQLDB
*
* Required Files:
* ---------------
* DependencyTable.h
* strongComponent.h
* XmlElement.h
* XmlDocument.h
*
* Build Process:
* --------------
* devenv CodeAnalyzer.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 7th March 2017
* - first release
*
*/

#ifndef PERSIST_H
#define PERSIST_H

#include "../NoSqlDB/noSqlDB.h"
#include "../DependencyTable/DependencyTable.h"
#include "../StrongComponents/strongComponent.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../FileSystem/FileSystem.h"


using Key = std::string;
using Keys = std::vector<Key>;
using StrData = std::string;
using namespace XmlProcessing;

using SPtr = std::shared_ptr<AbstractXmlElement>;
using defaultPath = std::string;

typedef Vertex<std::string, std::string> vertex;
using ComponentVector = std::vector<std::vector<vertex>>;

template <typename Data>
class Persist
{
public:
	void getData(DependencyTable&); // get the DependencyTable object
	void saveToNoSqlDB(); // saves to noSqlDB

	void getDataStrongComp(StrongComponents&);
	void saveToNoSqlDB_StrongComp();

	void displayNoSQLDB();
	std::string toXML();
	void persist(std::string);

private:
	DependencyTable depTable;
	StrongComponents strngComp;

	noSqlDB<StrData> db;
};


template<typename Data>
void Persist<Data>::persist(std::string path)
{
	std::cout << "\n\n===========Persisting the database ==========================\n\n";

	std::string xml = toXML();
	std::string _path;
	_path = "../xmlResults/";
	_path += path;

	std::ofstream out(_path);

	out << xml;
	std::cout << "\n\n\n=============================================================================";
	std::cout << "\n  Displaying Requirement 7: =============>";
	std::cout << "\n=============================================================================\n";
	std::cout << "\n Saving XML File at Location:===>>> \"" << FileSystem::Path::getFullFileSpec(_path) << "\"\n";
	out.close();


}

template <typename Data>
std::string Persist<Data>::toXML()
{
	std::string xml;
	XmlDocument doc;

	SPtr pRoot = makeTaggedElement("NoSqlDB");
	doc.docElement() = pRoot;
	Keys keys = db.keys();
	for (Key key : keys)
	{
		SPtr element = makeTaggedElement("Element");  // key
		pRoot->addChild(element);
		SPtr dbKeyElem = makeTaggedElement("Key");
		element->addChild(dbKeyElem);
		SPtr dbKeyValue = makeTextElement(key); // it is the text element

		dbKeyElem->addChild(dbKeyValue);
		SPtr dbValElem = makeTaggedElement("Value");  // value
		element->addChild(dbValElem);
		SPtr name_Tag = makeTaggedElement("Name");
		dbValElem->addChild(name_Tag);
		SPtr name_Value = makeTextElement(db.value(key).name);
		name_Tag->addChild(name_Value);
		SPtr category_Tag = makeTaggedElement("Category");
		dbValElem->addChild(category_Tag);
		SPtr category_Value = makeTextElement(db.value(key).category);
		category_Tag->addChild(category_Value);
		SPtr date_Tag = makeTaggedElement("TimeDate");
		dbValElem->addChild(date_Tag);
		SPtr date_Value = makeTextElement(db.value(key).timeDate);
		date_Tag->addChild(date_Value);
		SPtr data_Tag = makeTaggedElement("Data");
		dbValElem->addChild(data_Tag);
		SPtr data_Value = makeTextElement(db.value(key).data);
		data_Tag->addChild(data_Value);
		SPtr children_Tag = makeTaggedElement("Children");
		dbValElem->addChild(children_Tag);
		SPtr children_Value;
		std::string children_Vector_val = "";
		if (!db.value(key).children.empty())
			for (size_t i = 0; i < db.value(key).children.size(); i++)
			{
				SPtr child_Key_Tag = makeTaggedElement("Child_Key");
				children_Tag->addChild(child_Key_Tag);
				SPtr children_key_Value = makeTextElement(db.value(key).children[i]);
				child_Key_Tag->addChild(children_key_Value);
			}
	}
	xml = doc.toString();
	return xml;
}

template <typename Data>
void Persist<Data>::getData(DependencyTable& depTbl)
{
	depTable = depTbl;
}

template <typename Data>
void Persist<Data>::getDataStrongComp(StrongComponents& sc)
{
	strngComp = sc;
}

template <typename Data>
void Persist<Data>::displayNoSQLDB()
{
	std::cout << "\n\n\n==============================================";
	std::cout << "\n  Displaying Dependency Table in NoSqlDB :--> For Bonus \n";
	std::cout << "\n NOTE: Integrated Project #1 Completely, But have changed display function as some fields are not approprite to be saved in DB";
	std::cout << "\n==============================================\n";
	Keys db_keys = db.keys();
	for (Key key_db : db_keys)
	{
		std::cout << "\n    " << std::left << std::setw(8) << "Key" << " : " << key_db << "\n" << "\n    Value--> ";
		std::cout << db.value(key_db).show();
		std::cout << "----------------------------------------------------------------------------------------------------";
	}
}

template <typename Data>
void Persist<Data>::saveToNoSqlDB()
{
	ElementNoSqlDB<StrData> elemDB;
	ElementDep depElem;
	Keys keys = depTable.keys();
	for (Key key : keys)
	{
		depElem = depTable.value(key);
		elemDB.name = depElem.filename;
		for (auto depFiles : depElem.dependentFiles)
		{
			elemDB.children.push_back(depFiles);
		}
		db.save(key, elemDB);
		elemDB.children.clear();
	}
}


template <typename Data>
void Persist<Data>::saveToNoSqlDB_StrongComp()
{

	ComponentVector comp = strngComp.retrieveDisplayVector();

	int count = 1;


	for (auto component : comp)
	{
		ElementNoSqlDB<StrData> elemDB;
		elemDB.name = "StrongComponent: " + std::to_string(count);
		for (auto vrtx : component)
		{
			elemDB.children.push_back(vrtx.value().c_str());
		}
		db.save(elemDB.name, elemDB);
		elemDB.children.clear();
		count++;
	}

}
#endif 