#ifndef MACARONI_MODEL_BLOCK_H
#define MACARONI_MODEL_BLOCK_H

#include "../ME.h"
#include "BlockPtr.h"
#include "Member.h"
#include <string>

BEGIN_NAMESPACE2(Macaroni, Model)

/** A Block in the model is simply plain text associated with a node that is 
 * thrown in the direction of some generator.  Usually, blocks are directed 
 * towards the h or cpp generators, which place them in the files they're 
 * generating in the order they are found. */
class Block : public Member
{
friend void intrusive_ptr_add_ref(Block * p);
friend void intrusive_ptr_release(Block * p);
public:
	static BlockPtr Create(NodePtr host, const std::string & id, 
						   const std::string & block, 
						   const ReasonPtr reasonCreated);

	bool canBeChildOf(const Member *) const;

	inline const std::string & GetCode() const
	{
		return code;
	}

	inline const std::string & GetId() const
	{
		return id;
	}

private:
	Block(Node * host, const std::string & id, const std::string & code, const ReasonPtr reasonCreated);
	~Block();
	
	std::string code;	

	virtual const char * GetTypeName() const;

	std::string id;

	virtual void Visit(MemberVisitor * visitor) const;

};

END_NAMESPACE2

#endif
