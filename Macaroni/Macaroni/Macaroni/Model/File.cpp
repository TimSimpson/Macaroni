#ifndef MACARONI_FILE_CPP
#define MACARONI_FILE_CPP

#include "../ME.h"
#include "File.h"
#include "Class.h"
#include "Parser.spirit"
#include "ParserException.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

File::File(std::string & name)
:
classes(),
name(name)
{
	std::ifstream input(name.c_str());
	MACARONI_ASSERT(input.is_open(), "Could not open file.");
	
	std::stringstream ss;
	std::string line;
	while(!input.eof())
	{
		std::getline(input, line);
		ss << line << std::endl;
	}
	input.close();

	beginParse(ss.str());
	endParse();
}

File::~File()
{
}

void File::beginParse(std::string & contents)
{
	FileGrammar fileGrammar;	
	std::string stuff;
	ParserActions actions(*this);
	fileGrammar.actors = &actions;

	parse_info<> info = parse(
		contents.c_str(),
		fileGrammar,
		space_p);
}

void File::endParse()
{	
	for(std::vector<std::string>::iterator itr = this->comments.begin();
		itr != this->comments.end(); itr ++)
	{
		std::cout << "Comment = " << *itr << std::endl;
	}

	for(std::vector<ClassPtr>::iterator itr = this->classes.begin();
		itr != this->classes.end(); itr ++)
	{
		std::cout << "Class = " << (*itr)->GetName() 
			<< ", Extends=(";
		for(int i = 0; i < (*itr)->GetBaseClassCount(); i ++)
		{
			std::cout << (*itr)->GetBaseClass(i)->GetClass()->GetName() << ",";
		}
		std::cout << "),ns=" << ((*itr)->GetNamespace()->GetFullName())
			<< std::endl;
	}

	std::cout << std::endl;

	for(std::vector<ImportPtr>::iterator itr = this->imports.begin();
		itr != this->imports.end(); itr ++)
	{
		std::cout << "Import = " << (*itr)->GetName() 
			<< ", ns=" << ((*itr)->GetNamespace()->GetFullName())
			<< std::endl;
	}

	for(std::vector<NamespacePtr>::iterator itr = this->namespaces.begin();
		itr != this->namespaces.end(); itr ++)
	{
		std::cout << "Namespace = " << (*itr)->GetName() 
			<< ", fullName=" << ((*itr)->GetFullName())
			<< std::endl;
	}

	std::cout << "End of file parse." << std::endl;
}

std::string & File::GetName()
{
	return name;
}



END_NAMESPACE2


#endif
