#ifndef MACARONI_PARSER_CPP_CPPAXIOM_CPP
#define MACARONI_PARSER_CPP_CPPAXIOM_CPP

#include "CppAxiom.h"

BEGIN_NAMESPACE(Macaroni, Parser, Cpp)

CppAxiom::CppAxiom(const char * msg)
:msg(msg), refCount(0)
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
	return std::string(msg);
}

END_NAMESPACE

#endif