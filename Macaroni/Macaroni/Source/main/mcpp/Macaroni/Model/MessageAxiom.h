#ifndef MACARONI_MODEL_MESSAGEAXIOM_H
#define MACARONI_MODEL_MESSAGEAXIOM_H

#include "../ME.h"
#include "Axiom.h"

BEGIN_NAMESPACE2(Macaroni, Model)

class MessageAxiom : public Macaroni::Model::Axiom
{
public:
	MessageAxiom(const char * msgKey);	

	~MessageAxiom();

	static AxiomPtr Create(const char * msgKey);

	virtual const std::string ToString() const;

protected:
	
	virtual bool onDereference();

	virtual void onReference();

private:
	const char * msgKey;
	int refCount;
};

END_NAMESPACE2

#endif