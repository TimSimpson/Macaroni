#ifndef MACARONI_DOCUMENT_CPP
#define MACARONI_DOCUMENT_CPP

#include "../ME.h"
#include "Document.h"
#include "Class.h"
#include "ParserState.h"
#include "Parser.spirit"
#include "ParserException.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

Document::Document(const std::string & name)
:
classes(),
name(name),
parserState()
{
	///*std::ifstream input(name.c_str());
	//MACARONI_ASSERT(input.is_open(), "Could not open file.");
	//
	//std::stringstream ss;
	//std::string line;
	//while(!input.eof())
	//{
	//	std::getline(input, line);
	//	ss << line << std::endl;
	//}
	//input.close();

	//beginParse(ss.str());
	//endParse();*/
}


Document::~Document()
{
}

void Document::addNamespace(NamespacePtr ns)
{
	bool add = true;
	for(size_t i = 0; i < namespaces.size(); i ++)
	{
		if (namespaces[i] == ns)
		{
			add = false;
			break;
		}
	}	
	if (add)
	{
		namespaces.push_back(ns);
	}
}

void Document::beginParse(const std::string & contents)
{
	DocumentGrammar fileGrammar;	
	std::string stuff;
	ParserActions actions(*this, parserState);
	fileGrammar.actors = &actions;

	parse_info<> info = parse(
		contents.c_str(),
		fileGrammar,
		space_p);
}

void Document::endParse()
{	
	///*for(std::vector<std::string>::iterator itr = this->comments.begin();
	//	itr != this->comments.end(); itr ++)
	//{
	//	std::cout << "Comment = " << *itr << std::endl;
	//}

	//for(std::vector<ClassPtr>::iterator itr = this->classes.begin();
	//	itr != this->classes.end(); itr ++)
	//{
	//	std::cout << "Class = " << (*itr)->GetName() 
	//		<< ", Extends=(";
	//	for(int i = 0; i < (*itr)->GetBaseClassCount(); i ++)
	//	{
	//		std::cout << (*itr)->GetBaseClass(i)->GetClass()->GetName() << ",";
	//	}
	//	std::cout << "),ns=" << ((*itr)->GetNamespace()->GetFullName())
	//		<< std::endl;
	//}

	//std::cout << std::endl;

	//for(std::vector<ImportPtr>::iterator itr = this->imports.begin();
	//	itr != this->imports.end(); itr ++)
	//{
	//	std::cout << "Import = " << (*itr)->GetName() 
	//		<< ", ns=" << ((*itr)->GetNamespace()->GetFullName())
	//		<< std::endl;
	//}

	//for(std::vector<NamespacePtr>::iterator itr = this->namespaces.begin();
	//	itr != this->namespaces.end(); itr ++)
	//{
	//	std::cout << "Namespace = " << (*itr)->GetName() 
	//		<< ", fullName=" << ((*itr)->GetFullName())
	//		<< std::endl;
	//}

	//std::cout << "End of file parse." << std::endl;*/
}

std::string & Document::GetName()
{
	return name;
}

void Document::Read(const std::string & text)
{
	beginParse(text);
	endParse();
}



END_NAMESPACE2


#endif
