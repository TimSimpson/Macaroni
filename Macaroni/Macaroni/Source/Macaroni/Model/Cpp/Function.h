#ifndef MACARONI_MODEL_CPP_FUNCTION_H
#define MACARONI_MODEL_CPP_FUNCTION_H

#include "../../ME.h"
#include "Function.lh"
#include "../Member.h"
#include "../Reason.lh"
#include "ScopeMember.h"
#include "../Source.lh"
#include "../Type.h"
#include "Variable.lh"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents a function in any possible Scope. */
class Function : public ScopeMember
{
friend void intrusive_ptr_add_ref(Function * p);
friend void intrusive_ptr_release(Function * p);

public:

	//VariablePtr GetArgument(int index) const;

	//int GetArgumentCount() const;
	/** Clears the given list and fills it with the Argument nodes. */
	Model::NodeListPtr GetArguments() const;

	static FunctionPtr Create(NodePtr home, bool isInline, const Access access, const bool isStatic, const TypePtr rtnType, bool constMember, Model::ReasonPtr reason);

	virtual ~Function();

	virtual bool canBeChildOf(const Member * other) const;

	virtual bool DoesDefinitionReference(NodePtr node) const;

	const std::string & GetCodeBlock() const;

	virtual const char * GetTypeName() const;

	Model::NodePtr GetTypeNode() const;

	inline const TypePtr GetReturnType() const
	{
		return returnType;
	}

	inline bool IsConst() const
	{
		return constMember;
	}

	inline bool IsInline() const
	{
		return isInline;
	}

	/** Attaches code to this function. If code was already attached, throws a
	 * ModelInconsistencyException. */
	void SetCodeBlock(std::string & code, SourcePtr startOfCode);

	virtual void Visit(MemberVisitor * visitor) const;

protected:
	Function(Node * home, const char * typeName, Model::ReasonPtr reason, bool isInline, Access access, const bool isStatic, const TypePtr rtnType, bool constMember);

private:
	
	Function(Node * home, Model::ReasonPtr reason, bool isInline, Access access, const bool isStatic, const TypePtr rtnTypeInfo, bool constMember);

	bool codeAttached;

	std::string codeBlock;
	
	SourcePtr codeSource;

	bool constMember;

	const bool isInline;

	TypePtr returnType;
};

END_NAMESPACE

#endif
