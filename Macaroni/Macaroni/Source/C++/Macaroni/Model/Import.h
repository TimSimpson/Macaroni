#ifndef MACARONI_MODEL_IMPORT_H
#define MACARONI_MODEL_IMPORT_H

#include "../ME.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include "Node.lh"

BEGIN_NAMESPACE2(Macaroni, Model)

class Import
{
friend class ParserActions;
public:
	Import(NodePtr scope, std::string & name);
	
	const std::string & GetName() const { return name; }

	NodePtr GetNode() const { return scopePtr; }
private:
	std::string name;
	NodePtr scopePtr;
};

typedef boost::shared_ptr<Import> ImportPtr;

END_NAMESPACE2

#endif
