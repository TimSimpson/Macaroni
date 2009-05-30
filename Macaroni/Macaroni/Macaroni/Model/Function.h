#ifndef MACARONI_MODEL_FUNCTION_H
#define MACARONI_MODEL_FUNCTION_H

#include "../ME.h"
#include "Node.lh"
#include "ScopeMember.h"
#include "Type.lh"
#include <vector>

BEGIN_NAMESPACE2(Macaroni, Model)

class Function : public ScopeMember
{
public:
	Function(Node * scope, std::string & name);
	
	~Function(); 

	void AddArgument(TypePtr type);

	TypePtr GetArgument(int index) const;

	int GetArgumentCount() const;

	TypePtr GetReturnType() const;	

private:
	std::vector<Type *> argumentList;

	Type * returnType;
};

END_NAMESPACE2

#endif

