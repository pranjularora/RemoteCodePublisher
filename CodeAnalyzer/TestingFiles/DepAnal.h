#pragma once
// DepAnal.h

#include "../Parser/ActionsAndRules.h"
#include <iostream>
#include <unordered_set>
#include "../TypeTable/TypeTable.h"
#include "../DependencyTable/DependencyTable.h"
#include "../StrongComponents/strongComponent.h"
#include "../Graph/Graph.h"
using namespace GraphLib;

namespace CodeAnalysis
{
	typedef Vertex<std::string, std::string> vertex;
	typedef Display<std::string, std::string> display;

	class TypeAnal
	{
	public:
		using SPtr = std::shared_ptr<ASTNode*>;
		using Key = std::string;
		using Keys = std::vector<Key>;
		TypeAnal(TypeTable&, DependencyTable&);
		void doTypeAnal();
		void doDepAnal();
	private:
		void DFS(ASTNode* pNode, Element&, TypeTable&);
		std::stack<std::string> stack_Namespace;
		std::unordered_set<std::string> hashSet;
		AbstrSynTree& ASTref_;
		TypeTable& typeTable;
		DependencyTable& depTable;
		ScopeStack<ASTNode*> scopeStack_;
		Scanner::Toker& toker_;
	};

	inline TypeAnal::TypeAnal(TypeTable& typeTabl, DependencyTable& depTabl) :
		ASTref_(Repository::getInstance()->AST()),
		scopeStack_(Repository::getInstance()->scopeStack()),
		toker_(*(Repository::getInstance()->Toker())), typeTable(typeTabl), depTable(depTabl)
	{
	}


	inline void TypeAnal::DFS(ASTNode* pNode, Element& elem, TypeTable& typeTable)
	{
		if (pNode->type_.find("namespace") != std::string::npos && pNode->name_ == "Global Namespace")
		{
			stack_Namespace.push(pNode->name_);
		}
		static std::string path = "";
		if (pNode->path_ != path)
		{
			//std::cout << "\n    -- " << pNode->path_ << "\\" << pNode->package_;
			path = pNode->path_;
		}
		elem.type = pNode->type_;
		elem.path = pNode->path_;

		elem.pair_of_dep = std::make_pair(stack_Namespace.top(), pNode->package_);
		typeTable.save(pNode->name_, elem);

		hashSet.insert(pNode->path_);

		if (pNode->type_.find("namespace") != std::string::npos && pNode->name_ != "Global Namespace")
		{
			stack_Namespace.push(pNode->name_);
		}
		//std::cout << "\n  " << pNode->name_;
		//std::cout << ", " << pNode->type_;

		for (auto pChild : pNode->children_)
		{
			if (!(pChild->type_ == "function" && pNode->type_ != "namespace"))
			//if (!(pChild->type_ == "function"))
			{
				if(pChild->type_ == "class" || pChild->type_ == "function" || pChild->type_ == "struct" || pChild->type_ == "enum" || pChild->type_ == "namespace" || pChild->type_ == "typedef")
					DFS(pChild, elem, typeTable);
			}
		}
		if (pNode->type_.find("namespace") != std::string::npos)
			stack_Namespace.pop();
	}


	inline void TypeAnal::doTypeAnal()
	{
		ASTNode* pRoot = ASTref_.root();
		Element elem;
		DFS(pRoot, elem, typeTable);
		//typeTable.displayTypeTable();
	}

	inline void TypeAnal::doDepAnal()
	{
		for (auto path : hashSet)
		{
			ElementDep elemDep;
		//	std::cout << "\n\n" << path;
			std::ifstream in(path);
			if (in.good())
			{
				toker_.attach(&in);
				do
				{
					toker_.returnComments(false);
					std::string tok = toker_.getTok();
					if (typeTable.containsKey(tok))
					{
						//elem = typeTable.value(tok);
						for (auto elem : typeTable.value(tok))
						{
							std::string checkClass = elem.type.getValue();
							// tok key found has pkg name same as file on which tokenizer is executing..
							if(elem.pair_of_dep.second == FileSystem::Path::getName(path))
							//if (checkClass.compare("class") == 0 && path.find(elem.pair_of_dep.first) != std::string::npos)
							{
						//		std::cout << "\nsaving file name: " << tok;
								//elemDep.filename = tok;
								elemDep.filename = path;
							}
							else 
							{
							//	std::cout << "\nsaving dependency: " << tok << "\n displayin element: \n" << elem.path.getValue();
								//elemDep.dependentFiles.insert(tok);
								elemDep.dependentFiles.insert(elem.path.getValue());
							}
						}
					}
				} while (in.good());
				depTable.save(elemDep.filename, elemDep);
			}
			else if (path != "")
				std::cout << "\n\n\n  can't open " << path << "\n\n";
		}
	//	depTable.displayDependencyTable();
	}




}