#ifndef MACARONI_PARSER_MACARONI_MACARONIAXIOM_CPP
#define MACARONI_PARSER_MACARONI_MACARONIAXIOM_CPP

#include "MacaroniAxiom.h"
#include <Macaroni/Environment/Messages.h>

using Macaroni::Environment::Messages;

BEGIN_NAMESPACE(Macaroni, Parser, Macaroni)

MacaroniAxiom::MacaroniAxiom(const char * msgKey)
:msgKey(msgKey), refCount(0)
{
}

MacaroniAxiom::~MacaroniAxiom()
{
}

const std::string MacaroniAxiom::ToString() const
{
	return Messages::Get(msgKey);
}

bool MacaroniAxiom::onDereference()
{
	refCount --;
	return refCount > 0;
}

void MacaroniAxiom::onReference()
{
	refCount ++;
}

END_NAMESPACE

#endif