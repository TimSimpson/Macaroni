#ifndef MACARONI_REASON_H
#define MACARONI_REASON_H

#include "../ME.h"
#include "Axiom.h"
#include "ReasonPtr.h"
#include "Source.h"

BEGIN_NAMESPACE2(Macaroni, Model)

/** The reason why a Model object is constructed a certain way.  Client code
 * creates these objects and attaches them to Models which then manage them. */
class Reason
{
friend void intrusive_ptr_add_ref(Reason * p);
friend void intrusive_ptr_release(Reason * p);
public:	
	static ReasonPtr Create(AxiomPtr axiom, SourcePtr source);

	AxiomPtr GetAxiom();
	
	inline int GetReferenceCount()
	{
		return referenceCount;
	}

	SourcePtr GetSource();

	const std::string ToString() const;
protected:
	Reason(AxiomPtr axiom, SourcePtr source);
	~Reason();
private:	
	AxiomPtr axiom;
	SourcePtr source;
	int referenceCount;
};

END_NAMESPACE2

#endif
