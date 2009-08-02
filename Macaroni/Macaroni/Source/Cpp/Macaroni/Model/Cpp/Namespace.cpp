#ifndef MACARONI_MODEL_CPP_NAMESPACE_CPP
#define MACARONI_MODEL_CPP_NAMESPACE_CPP

#include "Namespace.h"
#include "../../Exception.h"
#include "../Node.h"
#include <memory>
#include <sstream>

using class Macaroni::Model::Node;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

Namespace::Namespace(Node * parent, ReasonPtr reason)
:Scope(parent, "Namespace", reason)
{
}

Namespace::~Namespace()
{
	
}

bool Namespace::canBeChildOf(const Member * other) const
{
	return dynamic_cast<const Namespace *>(other) != nullptr;
}

NamespacePtr Namespace::Create(NodePtr parent, ReasonPtr reason)
{
	return NamespacePtr(new Namespace(parent.get(), reason));
}

const char * Namespace::GetTypeName() const
{
	return "Namespace";
}

//
//Namespace * Namespace::create(std::string & name)
//{	
//	std::auto_ptr<Namespace> newNs(new Namespace(this, name));
//	addScopeMember(newNs.get());
//	return newNs.release();
//}

//NamespacePtr Namespace::CreatePrimitiveTypes(NamespacePtr parentNs, const std::string & name)
//{
//	NamespacePtr primitiveNs
//		= NamespacePtr(new Namespace(name));	
//	primitiveNs->selfPtr = primitiveNs;
//	Class::FindOrCreate(primitiveNs, std::string("char"));
//	Class::FindOrCreate(primitiveNs, std::string("double"));
//	Class::FindOrCreate(primitiveNs, std::string("float"));
//	Class::FindOrCreate(primitiveNs, std::string("int"));
//	return primitiveNs;
//}


//NamespacePtr Namespace::CreateRoot(const std::string & name)
//{
//	NamespacePtr rootNs
//		= NamespacePtr(new Namespace(name, NamespacePtr()));
//	CreatePrimitiveTypes(rootNs, std::string("{PRIMITIVES}"));
//	rootNs->root = NamespacePtr(rootNs);
//	rootNs->selfPtr = NamespacePtr;
//	rootNs->isRoot = true;
//	return rootNs;
//}

////NamespacePtr Namespace::Find(std::string & name)
////{
////	return NamespacePtr(find(name));
////}
////
////Namespace * Namespace::find(std::string & name)
////{
////	for(size_t i = 0; i < GetMemberCount(); i ++)
////	{
////		if (getMember(i)->GetName() == name)
////		{
////			return dynamic_cast<Namespace *>(getMember(i));
////		}
////	}
////	return nullptr;
////}
//
//ClassPtr Namespace::FindClass(std::string & name)
//{
//	for(size_t i = 0; i < classes.size(); i ++)
//	{
//		ClassPtr ptr = classes[i];
//		if (ptr->GetName() == name)
//		{
//			return ptr;
//		}
//	}
//	return ClassPtr();
//}

//NamespacePtr Namespace::FindOrCreate(std::string & name)
//{
//	// Find namespace for A
//	// MACARONI_ASSERT(root != false, "Root namespace in FindOrCreate cannot be nullptr.");
//
//	// For A::B::C, firstPart = A, lastPart = B::C
//	std::string firstPart;
//	std::string lastPart;
//	int index;
//	if ((index = name.find("::", 0)) == std::string::npos)
//	{
//		firstPart = name;
//		lastPart = "";
//	}
//	else
//	{
//		firstPart = name.substr(0, index);
//		lastPart = name.substr(index + 2);
//	}
//
//	
//	Namespace * ns = dynamic_cast<Namespace *>(this->find(firstPart));		
//	if (ns == nullptr)
//	{
//		ns = create(firstPart);		
//	}
//
//	if (lastPart.size() < 1)
//	{
//		return NamespacePtr(ns);
//	}
//	else
//	{
//		return ns->FindOrCreate(lastPart);
//	}
//}


//void Namespace::FindOrCreate(NamespacePtr startingNs, std::string & complexName,
//							 NamespacePtr & parentNamespace, 
//							 std::string & simpleName)
//{
//	MACARONI_ASSERT(startingNs != false, "Root namespace in FindOrCreate cannot be nullptr.");
//
//	int index = complexName.find_last_of("::");
//	if (index == std::string::npos)
//	{
//		parentNamespace = startingNs;
//		simpleName = complexName;
//	}
//	else
//	{
//		std::string additionalNsName = complexName.substr(0, index - 1);		
//		parentNamespace = startingNs->FindOrCreate(additionalNsName);
//		simpleName = complexName.substr(index + 1);
//	}
//}


//
//NamespacePtr Namespace::GetParent() const
//{
//	return dynamic_cast<Namespace *>(getNode());
//}
//
//NamespacePtr Namespace::GetRoot()
//{
//	return context->GetRootNamespace();
//}
//
//ClassPtr Namespace::InsertClass(ClassPtr newInstance)
//{
//	std::string className = newInstance->GetName();
//	MACARONI_ASSERT(FindClass(className) == false,
//					"Attempted to insert an already present class.");
//	classes.push_back(newInstance);
//	return newInstance;
//}

void intrusive_ptr_add_ref(Namespace * p)
{
	intrusive_ptr_add_ref((ScopeMember *)p);
}

void intrusive_ptr_release(Namespace * p)
{
	intrusive_ptr_release((ScopeMember *)p);
}




//
//NamespacePtr Namespace::GetRoot()
//{
//	static NamespacePtr root;
//	if (root == false)
//	{
//		root = NamespacePtr(new Namespace(std::string("{ROOT}"), NamespacePtr()));
//	}
//	
//	return root;
//}

void Namespace::Visit(MemberVisitor * visitor) const
{
	std::auto_ptr<MemberVisitor> nsVisitorDeleter(visitor->VisitNamespace(*this));
	MemberVisitor * nsVisitor = nsVisitorDeleter.get();
	if (nsVisitor == visitor)
	{
		nsVisitorDeleter.release(); // do not let auto_ptr destroy.
	}
	
	for(unsigned int i = 0; i < this->GetNode()->GetChildCount(); i ++)
	{
		NodePtr child = GetNode()->GetChild(i);
		if (child->GetMember() != nullptr)
		{
			child->GetMember()->Visit(nsVisitor);
		}
	}
}

END_NAMESPACE

#endif