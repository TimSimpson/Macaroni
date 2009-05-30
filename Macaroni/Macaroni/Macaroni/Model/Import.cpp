#ifndef MACARONI_MODEL_IMPORT_CPP
#define MACARONI_MODEL_IMPORT_CPP

#include "Import.h"
#include "Namespace.h"

BEGIN_NAMESPACE2(Macaroni, Model)

Import::Import(NodePtr startingNode, std::string & name)
:name(name), scopePtr(startingNode)
{
	/*startingNode
	Namespace::FindOrCreate(startingNs, name,
							this->namespacePtr, this->name);*/
	/////////*if (startingNs == false)
	////////{
	////////	startingNs = Namespace::GetRoot();
	////////}

	////////int index = name.find_last_of("::");
	////////if (index == std::string::npos)
	////////{
	////////	this->namespacePtr = startingNs;
	////////	this->name = name;
	////////}
	////////else
	////////{
	////////	std::string additionalNsName = name.substr(0, index - 1);
	////////	std::string lastName = name.substr(index + 1);		
	////////	this->namespacePtr = Namespace::FindOrCreate(startingNs, additionalNsName);
	////////	this->name = lastName;
	////////}*/
}

END_NAMESPACE2

#endif
