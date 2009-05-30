#ifndef MACARONI_MODEL_SCOPE_CPP
#define MACARONI_MODEL_SCOPE_CPP

#include "Scope.h"
#include "Class.h"
#include "Context.h"
#include "../Exception.h"
#include <memory>
#include "Namespace.h"
#include <sstream>
#include "UnknownScope.h"

BEGIN_NAMESPACE2(Macaroni, Model)

Scope::Scope(Scope * scope, const std::string & name)
:context(nullptr), ScopeMember(scope, name)
{
	if (scope != nullptr)
	{
		context = scope->context;
	}
}

Scope::~Scope()
{
	for(size_t i = 0; i < children.size(); i ++)
	{
		delete children[i];
	}
}

void Scope::addScopeMember(ScopeMember * member)
{
	children.push_back(member);
}

Class * Scope::createClass(const std::string & simpleName)
{	
	std::auto_ptr<Class> newInstance(new Class(this, simpleName));
	this->addScopeMember(newInstance.get());
	return newInstance.release();
}

Namespace * Scope::createNamespace(const std::string & simpleName)
{
	std::auto_ptr<Namespace> newInstance(new Namespace(this, simpleName));
	this->addScopeMember(newInstance.get());
	return newInstance.release();
}

Scope * Scope::createUnknownScope(const std::string & name)
{
	std::auto_ptr<UnknownScope> newScope(new UnknownScope(this, name));
	addScopeMember(newScope.get());
	return newScope.release();
}

ScopeMemberPtr Scope::Find(const std::string & name)
{
	ScopeMember * child = find(name);
	return ScopeMemberPtr(child);
}

ScopeMember * Scope::find(const std::string & name) const
{
	for(size_t i = 0; i < GetMemberCount(); i ++)
	{
		if (getMember(i)->GetName() == name)
		{
			return getMember(i);
		}
	}
	return nullptr;
}

ClassPtr Scope::FindOrCreateClass(const std::string & name)
{
	ScopePtr resultScope;
	std::string resultSimpleName;
	ParseComplexName(ScopePtr(this), name, resultScope, resultSimpleName);
	return ClassPtr(resultScope->createClass(resultSimpleName));
}

NamespacePtr Scope::FindOrCreateNamespace(const std::string & name)
{
	ScopePtr resultScope;
	std::string resultSimpleName;
	ParseComplexName(ScopePtr(this), name, resultScope, resultSimpleName);
	return NamespacePtr(resultScope->createNamespace(resultSimpleName));
}

ScopePtr Scope::FindOrCreateScope(const std::string & name)
{
	Scope * nakedPtr = findOrCreateScope(name);
	return ScopePtr(nakedPtr);
}

Scope * Scope::findOrCreateScope(const std::string & name)
{
	std::string firstPart;
	std::string lastPart;
	int index;
	if ((index = name.find("::", 0)) == std::string::npos)
	{
		firstPart = name;
		lastPart = "";
	}
	else
	{
		firstPart = name.substr(0, index);
		lastPart = name.substr(index + 2);
	}

	ScopeMember * member = this->find(firstPart);
	Scope * s = dynamic_cast<Scope *>(member);
	if (member != nullptr && s == nullptr)
	{
		std::stringstream ss;
		ss << "While searching for name \"" << name
			<< "\" in Scope \"" << GetFullName() << "\"-"
			<< "Can not create UnknownScope for name \""
			<< firstPart << "\"- found type which is ScopeMember, not Scope.";
		throw new Macaroni::Exception(ss.str().c_str());
	}

	if (s == nullptr)
	{
		s = createUnknownScope(firstPart);
	}

	if (lastPart.size() < 1)
	{
		return s;
	}
	else
	{
		return s->findOrCreateScope(lastPart);
	}
}
//
//ScopePtr Scope::FindScope(const std::string & complexName)
//{
//	std::vector<std::string> names;
//	SplitComplexName(complexName, names);
//	for(unsigned int i = 0; i < names.size(); i ++)
//	{
//		Scope = 
//	}
//}

size_t Scope::GetMemberCount() const
{
	return children.size();
}

void intrusive_ptr_add_ref(Scope * p)
{
	intrusive_ptr_add_ref(p->context);
}

void intrusive_ptr_release(Scope * p)
{
	intrusive_ptr_release(p->context);
}

bool Scope::IsComplexName(const std::string & name)
{
	return (name.find("::", 0) != std::string::npos);
}

bool Scope::IsSimpleName(const std::string & name)
{
	return (name.find("::", 0) == std::string::npos);
}

bool Scope::IsRoot() const
{
	return getScope() == nullptr;
}

void Scope::ParseComplexName(ScopePtr searchRoot, const std::string & complexName,
							 ScopePtr & resultScope,
							 std::string & resultSimpleName)
{
	MACARONI_ASSERT(searchRoot != false, "Root Scope of search can not be null.");
	
	int index = complexName.find_last_of("::");
	if (index == std::string::npos) // Not found
	{
		resultScope = searchRoot;		
		resultSimpleName = complexName;
	}
	else
	{
		std::string additionalScopeName;
		SplitScopeAndMemberName(complexName, additionalScopeName, resultSimpleName);		
		resultScope = searchRoot->FindOrCreateScope(additionalScopeName);
	}
}

void Scope::SplitFirstNameOffComplexName(const std::string & complexName,
									     std::string & firstPart,
										 std::string & lastPart)
{
	int index = complexName.find_first_of("::");
	if (index == std::string::npos) // Not found
	{
		lastPart = "";
		firstPart = complexName;
	}
	else
	{
		firstPart = complexName.substr(0, index);
		lastPart = complexName.substr(index + 2);
	}
}

void Scope::SplitScopeAndMemberName(const std::string & complexName,
			  				 std::string & scopeName, std::string & memberName)
{
	int index = complexName.find_last_of("::");
	if (index == std::string::npos) // Not found
	{
		scopeName = "";
		memberName = complexName;
	}
	else
	{
		scopeName = complexName.substr(0, index - 1);
		memberName = complexName.substr(index + 1);
	}
}

void Scope::SplitComplexName(const std::string & complexName,
			  				 std::vector<std::string> & subNames)
{
	MACARONI_ASSERT(complexName.length() < 16384, ":p < yeah right");
	unsigned int currentPos = 0;
	while(currentPos < complexName.length())
	{
		int index = complexName.find_first_of("::", currentPos);
		if (index == std::string::npos) // Not found
		{
			subNames.push_back(complexName.substr(currentPos));
			currentPos = complexName.length();
		}
		else
		{
			subNames.push_back(complexName.substr(0, index));
			currentPos = index + 2;
		}
	}
}

END_NAMESPACE2

#endif
