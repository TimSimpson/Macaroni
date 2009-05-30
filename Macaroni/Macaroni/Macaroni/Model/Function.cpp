#ifndef MACARONI_MODEL_FUNCTION_CPP
#define MACARONI_MODEL_FUNCTION_CPP

#include "Function.h"
#include "Type.h"

BEGIN_NAMESPACE2(Macaroni, Model)

Function::Function(Node * scope, std::string & name)
: ScopeMember(scope, name)
{
}

Function::~Function()
{
}

void Function::AddArgument(TypePtr type)
{
	argumentList.push_back(type.get());
}

TypePtr Function::GetArgument(int index) const
{
	return TypePtr(argumentList[index]);
}

int Function::GetArgumentCount() const
{
	return argumentList.size();
}

TypePtr Function::GetReturnType() const
{
	return TypePtr(returnType);
}

END_NAMESPACE2

#endif

