#ifndef MACARONI_PARSER_CPP_CPPAXIOM_H
#define MACARONI_PARSER_CPP_CPPAXIOM_H

#include "../../ME.h"
#include "../../Model/Axiom.h"

BEGIN_NAMESPACE(Macaroni, Parser, Cpp)

class CppAxiom : public Macaroni::Model::Axiom
{
public:
	CppAxiom(const char * msgKey);	

	~CppAxiom();

	virtual const std::string ToString() const;

protected:
	
	virtual bool onDereference();

	virtual void onReference();

private:
	const char * msgKey;
	int refCount;
};

END_NAMESPACE

#endif