#ifndef MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp_ClassParent_H
#define MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp_ClassParent_H

// This class was originally defined in Source/main/mcpp\Macaroni\Model\Cpp\ClassParent.mcpp
// Forward declaration necessary if this depends on anything which also depend on this.
namespace Macaroni { namespace Model { 
	class Node;
	namespace Cpp {
		class Class;
		class ClassParent;
	} 
} } // End namespace 

#include <Macaroni/Model/TypePtr.h>
#include <Macaroni/Model/Cpp/ClassPtr.h>
#include <Macaroni/Model/Cpp/Access.h>

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
private: 	Macaroni::Model::TypePtr  parent;
	private: ClassParent(Macaroni::Model::Cpp::Class *  node, Macaroni::Model::TypePtr  parent, Macaroni::Model::Cpp::Access  access, bool  _virtual);
public: 	inline Macaroni::Model::Cpp::Access  GetAccess() const	{
		
        return access;
    
	}
public: 	inline Macaroni::Model::Cpp::ClassPtr  GetChild() const	{
		
        return ClassPtr(child);
    
	}
public: 	Macaroni::Model::TypePtr  GetParent() const;
public: 	bool  IsVirtual() const;
}; // End of class ClassParent

} } } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp_ClassParent_H