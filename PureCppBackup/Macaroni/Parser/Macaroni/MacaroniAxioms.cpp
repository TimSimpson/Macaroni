#ifndef MACARONI_PARSER_MACARONI_MACARONIAXIOMS_CPP
#define MACARONI_PARSER_MACARONI_MACARONIAXIOMS_CPP

#include "MacaroniAxioms.h"
#include "MacaroniAxiom.h"
#include "../../Environment/Messages.h"

using Macaroni::Environment::Messages;

BEGIN_NAMESPACE(Macaroni, Parser, Macaroni)
namespace MacaroniAxioms
{

AxiomPtr AttributeImplicitDefinition()
{
	return AxiomPtr(new MacaroniAxiom("MacaroniAxioms.Attribute.Definition.Implicit"));
}

}

END_NAMESPACE

#endif