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
#ifndef MACARONI_MODEL_BLOCK_CPP
#define MACARONI_MODEL_BLOCK_CPP

#include "Block.h"
#include "MemberPtr.h"
#include "Member.h"
#include "ModelInconsistencyException.h"
#include "Node.h"
#include <sstream>

BEGIN_NAMESPACE2(Macaroni, Model)

void intrusive_ptr_add_ref(Block * p)
{
	intrusive_ptr_add_ref(dynamic_cast<Member *>(p));
}

void intrusive_ptr_release(Block * p)
{
	intrusive_ptr_release(dynamic_cast<Member *>(p));
}


Block::Block(Node * host, const std::string & id, const std::string & code, const ReasonPtr reasonCreated)
:	Member(host, "Block", reasonCreated),
	code(code),	
	id(id)
{
}

Block::~Block()
{
}

bool Block::canBeChildOf(const Member *) const
{
	return true;
}

BlockPtr Block::Create(NodePtr host, const std::string & id, 
					   const std::string & block, const ReasonPtr reasonCreated)
{
	ElementPtr existingMember = host->GetElement();
	if (!!existingMember)
	{
		BlockPtr existingBlock = boost::dynamic_pointer_cast<Block>(existingMember);
		std::stringstream ss;
		if (!existingBlock) 
		{
			ss << "The member at node " << host->GetFullName() 
				<< " was already defined as a "
				<< existingMember->GetTypeName()
				<< "; thus, it may not become a Block.";			
		}
		else
		{			
			ss << "The member at node " << host->GetFullName() 
				<< " was already created as a Block.  Blocks cannot be "
				<< "continued; please, create a new Block.";
		}
		throw Model::ModelInconsistencyException(existingMember->GetReasonCreated(),
									 			 reasonCreated, ss.str());
	}
	return BlockPtr(new Block(host.get(), id, block, reasonCreated));
}

const char * Block::GetTypeName() const
{
	return "Block";
}
	
void Block::Visit(MemberVisitor * visitor) const
{
	// do nothing.
}

END_NAMESPACE2

#endif
