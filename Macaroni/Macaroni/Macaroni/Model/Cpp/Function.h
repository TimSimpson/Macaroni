#ifndef MACARONI_MODEL_CPP_FUNCTION_H
#define MACARONI_MODEL_CPP_FUNCTION_H

#include "../../ME.h"
#include "Function.lh"
#include "../Member.h"
#include "../Reason.lh"
#include "ScopeMember.h"
#include "../Source.lh"
#include "TypeInfo.h"
#include "Variable.lh"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents a function in any possible Scope. */
class Function : public ScopeMember
{
friend void intrusive_ptr_add_ref(Function * p);
friend void intrusive_ptr_release(Function * p);

public:

	VariablePtr GetArgument(int index);

	int GetArgumentCount();

	static FunctionPtr Create(NodePtr home, const TypeInfo & rtnTypeInfo, Model::ReasonPtr reason);

	virtual ~Function();

	virtual bool canBeChildOf(const Member * other) const;

	virtual const char * GetTypeName() const;

	Model::NodePtr GetTypeNode() const;

	inline const TypeInfo GetReturnType() const
	{
		return returnTypeInfo;
	}

	/** Attaches code to this function. If code was already attached, throws a
	 * ModelInconsistencyException. */
	void SetCodeBlock(std::string & code, SourcePtr startOfCode);

private:
	
	Function(Node * home, Model::ReasonPtr reason, const TypeInfo & rtnTypeInfo);

	bool codeAttached;

	std::string codeBlock;
	
	SourcePtr codeSource;

	TypeInfo returnTypeInfo;
};

END_NAMESPACE

#endif
