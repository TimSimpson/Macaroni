#ifndef MACARONI_REASON_CPP
#define MACARONI_REASON_CPP

#include "Reason.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

Reason::Reason(AxiomPtr axiom, SourcePtr source)
:axiom(axiom), referenceCount(0), source(source)
{
}

Reason::~Reason()
{
}

ReasonPtr Reason::Create(AxiomPtr axiom, SourcePtr source)
{
	return ReasonPtr(new Reason(axiom, source));
}

AxiomPtr Reason::GetAxiom()
{
	return axiom;
}

SourcePtr Reason::GetSource()
{
	return source;
}


void intrusive_ptr_add_ref(Reason * p)
{
	p->referenceCount ++;
}

void intrusive_ptr_release(Reason * p)
{
	if ((-- (p->referenceCount)) < 1)
	{
		delete p;
	}
}

const std::string Reason::ToString() const
{
	std::stringstream ss;
	if (!!source)
	{
		ss << source->ToString();
	}
	else
	{
		ss << "[Source was null!]";
	}
	ss << ": ";
	if (!!axiom)
	{
		ss << axiom->ToString();
	}
	else
	{
		ss << "[Axiom was null!]";
	}	
	return ss.str();
}

END_NAMESPACE2

#endif
