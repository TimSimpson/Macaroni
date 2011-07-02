/**
 * Copyright 2011 Tim Simpson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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
