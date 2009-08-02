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

	VariablePtr GetArgument(int index) const;

	int GetArgumentCount() const;

	static FunctionPtr Create(NodePtr home, const TypeInfo & rtnTypeInfo, bool constMember, Model::ReasonPtr reason);

	virtual ~Function();

	virtual bool canBeChildOf(const Member * other) const;

	virtual bool DoesDefinitionReference(NodePtr node) const;

	const std::string & GetCodeBlock() const;

	virtual const char * GetTypeName() const;

	Model::NodePtr GetTypeNode() const;

	inline const TypeInfo GetReturnType() const
	{
		return returnTypeInfo;
	}

	inline bool IsConst() const
	{
		return constMember;
	}

	/** Attaches code to this function. If code was already attached, throws a
	 * ModelInconsistencyException. */
	void SetCodeBlock(std::string & code, SourcePtr startOfCode);

	virtual void Visit(MemberVisitor * visitor) const;

protected:
	Function(Node * home, const char * typeName, Model::ReasonPtr reason, const TypeInfo & rtnTypeInfo, bool constMember);

private:
	
	Function(Node * home, Model::ReasonPtr reason, const TypeInfo & rtnTypeInfo, bool constMember);

	bool codeAttached;

	std::string codeBlock;
	
	SourcePtr codeSource;

	bool constMember;

	TypeInfo returnTypeInfo;
};

END_NAMESPACE

#endif