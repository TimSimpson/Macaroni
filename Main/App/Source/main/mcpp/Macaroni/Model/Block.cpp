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

#include <Macaroni/Model/ElementPtr.h>
#include <boost/foreach.hpp>
#include "ModelInconsistencyException.h"
#include "Node.h"
#include <boost/optional.hpp>
#include <sstream>
#include <Macaroni/Model/Project/Target.h>
#include <Macaroni/Core/Visitor.h>

using Macaroni::Core::BaseVisitor;
using boost::optional;
using Macaroni::Model::Project::Target;
using Macaroni::Model::Project::TargetPtr;


BEGIN_NAMESPACE2(Macaroni, Model)

void intrusive_ptr_add_ref(Block * p)
{
	intrusive_ptr_add_ref(dynamic_cast<Element *>(p));
}

void intrusive_ptr_release(Block * p)
{
	intrusive_ptr_release(dynamic_cast<Element *>(p));
}


Block::Block(Target * target, Node * host, const std::string & id,
			 const std::string & code, const ReasonPtr reasonCreated,
			 optional<ImportList> importedNodes)
:	Element(host, reasonCreated),
	code(code),
	id(id),
	imports(),
	target(target)
{
	// Target usually isn't needed unless this is a ~block placed
	// directly within a unit. Blocks inside larger nodes like
	if (nullptr != target)
	{
		target->AddElement(this);
	}
	if (importedNodes)
	{
		imports = importedNodes.get();
	}
}

Block::~Block()
{
}

bool Block::Accept(BaseVisitor & v)
{
    return Visit(*this, v);
}

BlockPtr Block::Create(TargetPtr target, NodePtr host, const std::string & id,
					   const std::string & block, const ReasonPtr reasonCreated,
					   optional<ImportList> importedNodes)
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
	return BlockPtr(new Block(target.get(), host.get(), id, block, reasonCreated, importedNodes));
}

const ImportList & Block::GetImportedNodes() const
{
	return imports;
}

TargetPtr Block::GetOwner() const
{
	if (!target)
	{
		return Element::GetOwner();
	}
	return target;
}

const char * Block::GetTypeName() const
{
	return "Block";
}

bool Block::OwnedBy(TargetPtr targetArg) const
{
	if (!this->target)
	{
		// Defer to base class behavior.
		return Element::OwnedBy(targetArg);
	}
	if (!targetArg)
	{
		return false;
	}
	if (*targetArg == *(this->target))
	{
		return true;
	}
	return this->target->OwnedBy(targetArg.get());
}

bool Block::RequiresCppFile() const
{
	// cpp or cpp-include necessitate a cpp file.
	if (id.size() < 3)
	{
		return false;
	}
	std::string sub = id.substr(0, 3);
	return sub == "cpp";
}

bool Block::RequiresHFile() const
{
	// h, h-predef, or h-postdef all necessitate an h file.
	return id.size() > 0 && id[0] == 'h';
}

END_NAMESPACE2

#endif
