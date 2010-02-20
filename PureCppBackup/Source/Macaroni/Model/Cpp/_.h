#ifndef MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp___H
#define MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp___H
/* ~ Debug Output of Dependency Graph ~ */
/* Macaroni::Model::Cpp::ClassParentPtr
     ld    : namespace Macaroni { namespace Model { namespace Cpp { 
typedef boost::intrusive_ptr<Macaroni::Model::Cpp::ClassParent >  ClassParentPtr;} } } // End namespace 
 
     hd    : #include <Macaroni/Model/Cpp/_.h>
 
     using : using Macaroni::Model::Cpp::ClassParentPtr;
 
     light dependencies : 
                   Macaroni::Model::Cpp::ClassParent

     HEAVY dependencies : 
                   boost::intrusive_ptr
*/
/* Macaroni::Model::Cpp::ClassParentList
     ld    : namespace Macaroni { namespace Model { namespace Cpp { 
typedef std::vector<Macaroni::Model::Cpp::ClassParent >  ClassParentList;} } } // End namespace 
 
     hd    : #include <Macaroni/Model/Cpp/_.h>
 
     using : using Macaroni::Model::Cpp::ClassParentList;
 
     light dependencies : 
                   Macaroni::Model::Cpp::ClassParent

     HEAVY dependencies : 
                   std::vector
*/
/* Macaroni::Model::Cpp::ClassParentListPtr
     ld    : namespace Macaroni { namespace Model { namespace Cpp { 
typedef boost::intrusive_ptr<Macaroni::Model::Cpp::ClassParentList >  ClassParentListPtr;} } } // End namespace 
 
     hd    : #include <Macaroni/Model/Cpp/_.h>
 
     using : using Macaroni::Model::Cpp::ClassParentListPtr;
 
     light dependencies : 
                   Macaroni::Model::Cpp::ClassParentList
                   Macaroni::Model::Cpp::ClassParent

     HEAVY dependencies : 
                   boost::intrusive_ptr
                   std::vector
*/

#include <vector>
#include <boost/shared_ptr.hpp>
#include <Macaroni/Model/Cpp/ClassParent.h>
namespace Macaroni { namespace Model { namespace Cpp { 
class ClassParent;
} } } // End namespace 
#include <vector>
#include <Macaroni/Model/Cpp/_.h>
#include <Macaroni/Model/Cpp/ClassParent.h>
namespace Macaroni { namespace Model { namespace Cpp { 
class ClassParent;
} } } // End namespace 
#include <boost/shared_ptr.hpp>
#include <Macaroni/Model/Cpp/ClassParent.h>
namespace Macaroni { namespace Model { namespace Cpp { 
class ClassParent;
} } } // End namespace 
namespace Macaroni { namespace Model { namespace Cpp { 
typedef std::vector<Macaroni::Model::Cpp::ClassParent >  ClassParentList;} } } // End namespace 

namespace Macaroni { namespace Model { namespace Cpp { 

typedef boost::intrusive_ptr<Macaroni::Model::Cpp::ClassParent > ClassParentPtr;
typedef std::vector<Macaroni::Model::Cpp::ClassParent > ClassParentList;
typedef boost::intrusive_ptr<Macaroni::Model::Cpp::ClassParentList > ClassParentListPtr;

} } } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp___H