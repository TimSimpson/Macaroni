#ifndef MACARONI_MODEL_CPP_FUNCTION_H
#define MACARONI_MODEL_CPP_FUNCTION_H

#include "../../ME.h"
#include "Function.lh"
#include "../Member.h"
#include "../Reason.lh"
#include "ScopeMember.h"
#include "TypeInfo.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents a function in any possible Scope. */
class Function : public ScopeMember
{
friend void intrusive_ptr_add_ref(Function * p);
friend void intrusive_ptr_release(Function * p);

public:

	static FunctionPtr Create(NodePtr home, const TypeInfo & rtnTypeInfo, Model::ReasonPtr reason);

	virtual ~Function();

	virtual bool canBeChildOf(const Member * other) const;

	virtual const char * GetTypeName() const;

	Model::NodePtr GetTypeNode() const;

	inline const TypeInfo GetReturnType() const
	{
		return returnTypeInfo;
	}

private:
	
	Function(Node * home, Model::ReasonPtr reason, const TypeInfo & rtnTypeInfo);

	TypeInfo returnTypeInfo;
};

END_NAMESPACE

#endif
