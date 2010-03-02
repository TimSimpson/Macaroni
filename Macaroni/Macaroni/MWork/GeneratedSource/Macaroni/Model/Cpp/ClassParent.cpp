#ifndef MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp_ClassParent_CPP
#define MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp_ClassParent_CPP

#include "ClassParent.h"
#include <Macaroni/Model/Cpp/Access.h>
#include <Macaroni/Model/Cpp/Class.h>
#include <Macaroni/Model/Cpp/ClassPtr.h>
#include <boost/shared_ptr.hpp>
#include <Macaroni/Model/Node.h>
#include <Macaroni/Model/_.h>
#include <vector>

using Macaroni::Model::Cpp::Access;
using Macaroni::Model::Cpp::Class;
using Macaroni::Model::Cpp::ClassPtr; 
using boost::intrusive_ptr;
using Macaroni::Model::Node;
// ~ <(I don't know how to generate a using statement for Macaroni::Model::NodePtr.) 
using std::vector;

namespace Macaroni { namespace Model { namespace Cpp { 

/* Adopted Global Functions */
namespace {
} // end anonymous namespace
void  intrusive_ptr_add_ref(ClassParent *  p)
{
	
        intrusive_ptr_add_ref(p->child);
    
}
void  intrusive_ptr_release(ClassParent *  p)
{
	
        intrusive_ptr_release(p->child);
    
}
/* End globals. */

ClassParent::ClassParent(Class *  node, Node *  parent, Access  access, bool  _virtual)
: access(access), child(node), parent(parent), _virtual(_virtual)
{
	
    
}
//~<(Skipping inline function "Macaroni::Model::Cpp::ClassParent::GetAccess")

} } } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp_ClassParent_CPP