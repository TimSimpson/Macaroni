#ifndef MACARONI_MODEL_NAMESPACE_H
#define MACARONI_MODEL_NAMESPACE_H

#include "../ME.h"
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "Class.lh"
#include "Namespace.lh"
#include "Node.h"

BEGIN_NAMESPACE2(Macaroni, Model)

class Namespace : public Node
{
friend class Context;
friend struct NamespaceLuaFunctions;
friend class Node;
//friend class ParserActions;
friend void intrusive_ptr_add_ref(Namespace * p);
friend void intrusive_ptr_release(Namespace * p);
public:	
	/** Creates a Namespace with classes standing in for C++ primitives such as
	 * "char" or "int." */
	//static NamespacePtr CreatePrimitiveTypes(NamespacePtr parentNs, const std::string & name);

	///*static NamespacePtr CreateRoot(const std::string & name);*/

	//NamespacePtr FindOrCreate(std::string & name);

	/**
	 * Given A::B::C,
	 * parentNameSpace is the namespace A::B and simpleName is "C".
	 */
	/*static void FindOrCreate(NamespacePtr root, std::string & complexName,
							 NamespacePtr & parentNamespace, 
							 std::string & simpleName);*/
	
	//NamespacePtr Find(std::string & name);

	ClassPtr FindClass(std::string & name);

	NamespacePtr GetParent() const;

	/** Gets the root at the core of this Namespace tree. */
	NamespacePtr GetRoot();

	ClassPtr InsertClass(ClassPtr newInstance);

	//static NamespacePtr GetRoot();

	virtual ~Namespace();

protected:	

private:
	Namespace(Node * parent, const std::string & name);	

	std::vector<ClassPtr> classes;

	/** Assumes the given namespace does not exist and creates it as a child
	 * of this namespace. */
	Namespace * create(std::string & name);

	//Namespace * find(std::string & name);
};

void intrusive_ptr_add_ref(Namespace * p);

void intrusive_ptr_release(Namespace * p);

END_NAMESPACE2

#endif
