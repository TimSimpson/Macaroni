#ifndef MACARONI_MODEL_CPP_FUNCTIONOVERLOAD_H
#define MACARONI_MODEL_CPP_FUNCTIONOVERLOAD_H

#include "../../ME.h"
#include "Function.h"
#include "FunctionOverloadPtr.h"
#include "../Member.h"
#include "../ReasonPtr.h"
#include "ScopeMember.h"
#include "../SourcePtr.h"
#include "../Type.h"
#include "VariablePtr.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

/** Represents a function in any possible Scope. */
class FunctionOverload : public ScopeMember
{
friend void intrusive_ptr_add_ref(FunctionOverload * p);
friend void intrusive_ptr_release(FunctionOverload * p);

public:
	static FunctionOverloadPtr Create(NodePtr home, bool isInline, 
									  const Access access, 
									  const bool isStatic, const TypePtr rtnType, 
									  bool constMember, Model::ReasonPtr reason);

	static FunctionOverloadPtr Create(FunctionPtr home, bool isInline, 
									  const Access access, 
									  const bool isStatic, const TypePtr rtnType, 
									  bool constMember, Model::ReasonPtr reason);

	static FunctionOverloadPtr Create(Function * home, bool isInline, 
									  const Access access, 
									  const bool isStatic, const TypePtr rtnType, 
									  bool constMember, Model::ReasonPtr reason);

	virtual ~FunctionOverload();
	

	/** Clears the given list and fills it with the Argument nodes. */
	Model::NodeListPtr GetArguments() const;

	virtual bool DoesDefinitionReference(NodePtr node) const;

	const std::string & GetCodeBlock() const;
	
	inline const TypePtr GetReturnType() const
	{
		return returnType;
	}

	virtual const char * GetTypeName() const;

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
	//FunctionOverload(Node home, bool isInline, const Access access, const bool isStatic, const TypePtr rtnType, bool constMember, Model::ReasonPtr reason);
	
	FunctionOverload(Node * home, Model::ReasonPtr reason, bool isInline, Access access, const bool isStatic, const TypePtr rtnTypeInfo, bool constMember);

	FunctionOverload(Node * home, const char * typeName, Model::ReasonPtr reason, bool isInline, Access access, const bool isStatic, const TypePtr rtnTypeInfo, bool constMember);

	virtual bool canBeChildOf(const Member * other) const;

private:		

	bool codeAttached;

	std::string codeBlock;
	
	SourcePtr codeSource;

	bool constMember;

	const bool isInline;

	Macaroni::Model::ReasonPtr reasonCreated;

	TypePtr returnType;
};

END_NAMESPACE

#endif
