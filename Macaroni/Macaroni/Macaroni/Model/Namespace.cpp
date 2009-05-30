#ifndef MACARONI_MODEL_NAMESPACE_CPP
#define MACARONI_MODEL_NAMESPACE_CPP

#include "../Exception.h"
#include "Context.h"
#include "Namespace.h"
#include "Class.h"
#include <memory>
#include <sstream>


BEGIN_NAMESPACE2(Macaroni, Model)

Namespace::Namespace(Node * parent, const std::string & name)
:Node(parent, name)
{
	MACARONI_ASSERT(Node::IsSimpleName(name), 
				    "Namespace constructor cannot take complex name as argument.");
}

Namespace::~Namespace()
{
	
}

Namespace * Namespace::create(std::string & name)
{
	std::auto_ptr<Namespace> newNs(new Namespace(this, name));
	addScopeMember(newNs.get());
	return newNs.release();
}

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

ClassPtr Namespace::FindClass(std::string & name)
{
	for(size_t i = 0; i < classes.size(); i ++)
	{
		ClassPtr ptr = classes[i];
		if (ptr->GetName() == name)
		{
			return ptr;
		}
	}
	return ClassPtr();
}

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



NamespacePtr Namespace::GetParent() const
{
	return dynamic_cast<Namespace *>(getNode());
}

NamespacePtr Namespace::GetRoot()
{
	return context->GetRootNamespace();
}

ClassPtr Namespace::InsertClass(ClassPtr newInstance)
{
	std::string className = newInstance->GetName();
	MACARONI_ASSERT(FindClass(className) == false,
					"Attempted to insert an already present class.");
	classes.push_back(newInstance);
	return newInstance;
}

void intrusive_ptr_add_ref(Namespace * p)
{
	intrusive_ptr_add_ref(static_cast<Node *>(p));
}

void intrusive_ptr_release(Namespace * p)
{
	intrusive_ptr_release(static_cast<Node *>(p));
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

END_NAMESPACE2

#endif
