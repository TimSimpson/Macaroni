#ifndef MACARONI_PARSER_CPP_CPPAXIOMS_CPP
#define MACARONI_PARSER_CPP_CPPAXIOMS_CPP

#include "CppAxioms.h"
#include "CppAxiom.h"

BEGIN_NAMESPACE(Macaroni, Parser, Cpp)
namespace CppAxioms
{

AxiomPtr NamespaceCreation()
{
	return AxiomPtr(new CppAxiom("\"namespace\" keyword begins or continues namespaces."));
}

}// end of CppAxioms namespace
END_NAMESPACE

#endif