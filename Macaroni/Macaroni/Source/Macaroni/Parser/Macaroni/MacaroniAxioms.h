#ifndef MACARONI_PARSER_MACARONI_MACARONIAXIOMS_H
#define MACARONI_PARSER_MACARONI_MACARONIAXIOMS_H

#include "../../ME.h"
#include "../../Model/Axiom.h"

using Macaroni::Model::Axiom;
using Macaroni::Model::AxiomPtr;

BEGIN_NAMESPACE(Macaroni, Parser, Macaroni)

namespace MacaroniAxioms
{
	AxiomPtr AttributeImplicitDefinition();	
};

END_NAMESPACE

#endif