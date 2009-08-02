#ifndef MACARONI_MODEL_CPP_CLASS_H
#define MACARONI_MODEL_CPP_CLASS_H

#include "../../ME.h"
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "../Member.h"
#include "Class.lh"
#include "Scope.h"

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

class Class : public Scope
{
friend void intrusive_ptr_add_ref(Class * p);
friend void intrusive_ptr_release(Class * p);
public:	
	static ClassPtr Create(NodePtr home, Model::NodeListPtr importedNodes, ReasonPtr reason);

	virtual ~Class();

	void AddFriend(NodePtr node);
	

	virtual bool canBeChildOf(const Member * other) const;

	virtual bool DoesDefinitionReference(NodePtr node) const;

	NodeListPtr GetFriendNodes() const;

	NodeListPtr GetImportedNodes() const;

	virtual const char * GetTypeName() const;
	
	virtual void Visit(MemberVisitor * visitor) const;

protected:	
	Class(Node * home, Model::NodeListPtr importedNodes, ReasonPtr reason);
private:

	NodeListPtr imports;

	NodeListPtr friends;
	
};



END_NAMESPACE

#endif