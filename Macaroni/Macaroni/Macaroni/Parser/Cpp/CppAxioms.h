#ifndef MACARONI_PARSER_CPP_CPPAXIOMS_H
#define MACARONI_PARSER_CPP_CPPAXIOMS_H

#include "../../ME.h"
#include "../../Model/Axiom.h"

using Macaroni::Model::Axiom;
using Macaroni::Model::AxiomPtr;

BEGIN_NAMESPACE(Macaroni, Parser, Cpp)

namespace CppAxioms
{
	AxiomPtr NamespaceCreation();
};

END_NAMESPACE

#endif