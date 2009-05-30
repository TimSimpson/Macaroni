#ifndef MACARONI_MODEL_IMPORT_H
#define MACARONI_MODEL_IMPORT_H

#include "../ME.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include "Scope.lh"

BEGIN_NAMESPACE2(Macaroni, Model)

class Import
{
friend class ParserActions;
public:
	Import(ScopePtr scope, std::string & name);
	
	const std::string & GetName() const { return name; }

	ScopePtr GetScope() const { return scopePtr; }
private:
	std::string name;
	ScopePtr scopePtr;
};

typedef boost::shared_ptr<Import> ImportPtr;

END_NAMESPACE2

#endif
