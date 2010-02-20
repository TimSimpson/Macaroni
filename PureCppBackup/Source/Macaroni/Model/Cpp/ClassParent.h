#ifndef MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp_ClassParent_H
#define MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp_ClassParent_H

// This class was originally defined in Source/Macaroni/Model/Cpp/ClassParent.mcpp
// Forward declaration necessary if this depends on anything which also depend on this.
namespace Macaroni { namespace Model { namespace Cpp { 
class ClassParent;
} } } // End namespace 

#include <Macaroni/Model/Node.h>
#include <Macaroni/Model/Cpp/Access.h>
#include <Macaroni/Model/Cpp/Class.h>

namespace Macaroni { namespace Model { namespace Cpp { 


/* Public Global Methods */
void  intrusive_ptr_add_ref(Macaroni::Model::Cpp::ClassParent *  p);
void  intrusive_ptr_release(Macaroni::Model::Cpp::ClassParent *  p);

class ClassParent
{
friend Macaroni::Model::Cpp::Class;
friend 	void  intrusive_ptr_add_ref(Macaroni::Model::Cpp::ClassParent *  p);
friend 	void  intrusive_ptr_release(Macaroni::Model::Cpp::ClassParent *  p);
private: 	Macaroni::Model::Cpp::Access  access;
private: 	Macaroni::Model::Cpp::Class *  child;
private: 	bool  _virtual;
private: 	Macaroni::Model::Node *  parent;
	private: ClassParent(Macaroni::Model::Cpp::Class *  node, Macaroni::Model::Node *  parent, Macaroni::Model::Cpp::Access  access, bool  _virtual);
public: 	inline Macaroni::Model::Cpp::Access  GetAccess() const	{
		
        return access;
    
	}
}; // End of class ClassParent

} } } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp_ClassParent_H