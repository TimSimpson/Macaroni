#ifndef MACARONI_PARSER_CPP_CPPAXIOMS_H
#define MACARONI_PARSER_CPP_CPPAXIOMS_H

#include "../../ME.h"
#include "../../Model/Axiom.h"

using Macaroni::Model::Axiom;
using Macaroni::Model::AxiomPtr;

BEGIN_NAMESPACE(Macaroni, Parser, Cpp)

namespace CppAxioms
{
	AxiomPtr ClassCreation();
	AxiomPtr CtorCreation();
	AxiomPtr DtorCreation();
	AxiomPtr FunctionCreation();
	AxiomPtr NamespaceCreation();
	AxiomPtr PrimitiveChar();
	AxiomPtr PrimitiveDouble();
	AxiomPtr PrimitiveFloat();
	AxiomPtr PrimitiveSignedInt();
	AxiomPtr PrimitiveUnsignedInt();
	AxiomPtr VariableScopeCreation();
};

END_NAMESPACE

#endif