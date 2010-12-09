#ifndef MACARONI_MODEL_MESSAGEAXIOM_CPP
#define MACARONI_MODEL_MESSAGEAXIOM_CPP

#include "MessageAxiom.h"
#include "../Environment/Messages.h"

using Macaroni::Environment::Messages;

BEGIN_NAMESPACE2(Macaroni, Model)

MessageAxiom::MessageAxiom(const char * msgKey)
:msgKey(msgKey), refCount(0)
{
}

MessageAxiom::~MessageAxiom()
{
}

AxiomPtr MessageAxiom::Create(const char * msgKey)
{
	return AxiomPtr(new MessageAxiom(msgKey));
}

bool MessageAxiom::onDereference()
{
	refCount --;
	return refCount > 0;
}

void MessageAxiom::onReference()
{
	refCount ++;
}

const std::string MessageAxiom::ToString() const
{
	return Messages::Get(msgKey);
}

END_NAMESPACE2

#endif