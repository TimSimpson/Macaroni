#ifndef MACARONI_PARSER_MACARONI_MACARONIAXIOM_H
#define MACARONI_PARSER_MACARONI_MACARONIAXIOM_H

#include <Macaroni/ME.h>
#include <Macaroni/Model/Axiom.h>

BEGIN_NAMESPACE(Macaroni, Parser, Macaroni)

class MacaroniAxiom : public Model::Axiom
{
public:
	MacaroniAxiom(const char * msgKey);	

	~MacaroniAxiom();

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