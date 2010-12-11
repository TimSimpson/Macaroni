#ifndef MACARONI_PARSER_CPP_CPPAXIOM_CPP
#define MACARONI_PARSER_CPP_CPPAXIOM_CPP

#include "CppAxiom.h"
#include "../../Environment/Messages.h"

using Macaroni::Environment::Messages;

BEGIN_NAMESPACE(Macaroni, Parser, Cpp)

CppAxiom::CppAxiom(const char * msgKey)
:msgKey(msgKey), refCount(0)
{
}

CppAxiom::~CppAxiom()
{
}

bool CppAxiom::onDereference()
{
	refCount --;
	return refCount > 0;
}

void CppAxiom::onReference()
{
	refCount ++;
}

const std::string CppAxiom::ToString() const
{
	return Messages::Get(msgKey);
}

END_NAMESPACE

#endif