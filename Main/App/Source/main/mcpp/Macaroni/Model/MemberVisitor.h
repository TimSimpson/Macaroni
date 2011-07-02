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
#ifndef MACARONI_MODEL_MEMBERVISITOR_H
#define MACARONI_MODEL_MEMBERVISITOR_H

#include "../ME.h"
#include "Member.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Class;
class Constructor;
class Destructor;
class Function;
class Namespace;
class Variable;

END_NAMESPACE


BEGIN_NAMESPACE2(Macaroni, Model)

/** Visits a Member.  This can be used to implement behaviors that iterate the
 * entire Macaroni Model but can be called together, so that Nodes are iterated
 * by all Visitors in the same way, at the same time.
 * A note about the functions returning pointers: if the returned object differs
 * from the one that was called, it is deleted by the Member after the Member's
 * children are traversed.  Be careful!
 */
class MemberVisitor
{
public:
	virtual ~MemberVisitor(){};
	
	virtual MemberVisitor * VisitClass(const Cpp::Class &) = 0;

	virtual void VisitClassFooter() = 0;

	virtual void VisitConstructor(const Cpp::Constructor &) = 0;

	virtual void VisitDestructor(const Cpp::Destructor &) = 0;

	virtual void VisitAdoptedFunction(const Cpp::Function &) = 0;

	virtual void VisitFunction(const Cpp::Function &) = 0;

	virtual MemberVisitor * VisitNamespace(const Cpp::Namespace &) = 0;

	virtual void VisitAdoptedVariable(const Cpp::Variable &) = 0;

	virtual void VisitVariable(const Cpp::Variable &) = 0;
	
protected:
	MemberVisitor(){}

private:
};

END_NAMESPACE2

#endif

