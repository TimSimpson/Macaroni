#ifndef MACARONI_MODEL_CLASS_H
#define MACARONI_MODEL_CLASS_H

#include "../ME.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include <vector>
#include "Namespace.lh"
#include "Scope.h"
#include "Type.h"

#include "Class.lh"

BEGIN_NAMESPACE2(Macaroni, Model)

class File;

class BaseClass
{
public:
	enum Access
	{
		Public,
		Protected,
		Private
	};
	BaseClass(Access access, ClassPtr baseClass);
	Access GetAccess() const { return access; }
	ClassPtr GetClass() const { return baseClass; }
private:
	ClassPtr baseClass;
	Access access;
};

typedef boost::shared_ptr<BaseClass> BaseClassPtr;

class Class : public Scope, public Type
{
friend void intrusive_ptr_add_ref(Class * p);
friend void intrusive_ptr_release(Class * p);
friend class Scope;
//friend class Namespace;
//friend class ParserActions;
public:	
	void AddBaseClass(BaseClassPtr baseClass);

	static ClassPtr FindOrCreate(NamespacePtr startingNs, std::string & complexName);

	/** False if missing class body, base classes, etc. */
	bool IsDefined();

	BaseClassPtr GetBaseClass(int index) { return baseClasses[index]; }

	int GetBaseClassCount() const { return baseClasses.size(); }

	virtual const std::string & GetTypeName() const { return GetName(); }
	
	virtual ScopePtr GetTypeScope() const { return GetScope(); }
	
	void SetBody(std::string & body);

private:
	Class(Scope * scope, const std::string & name);
	std::vector<BaseClassPtr> baseClasses;
	//std::string name;
	//NamespacePtr namespacePtr;
	std::string body;
};

void intrusive_ptr_add_ref(Class * p);

void intrusive_ptr_release(Class * p);

END_NAMESPACE2

#endif
