#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build___H
#define MACARONI_COMPILE_GUARD_Macaroni_Build___H
/* ~ Debug Output of Dependency Graph ~ */
/* Macaroni::Build::BuildContextPtr
     ld    : namespace Macaroni { namespace Build { 
typedef boost::shared_ptr<Macaroni::Build::BuildContext >  BuildContextPtr;} } // End namespace 
 
     hd    : #include <Macaroni/Build/_.h>
 
     using : using Macaroni::Build::BuildContextPtr;
 
     light dependencies : 
                   Macaroni::Build::BuildContext

     HEAVY dependencies : 
                   boost::shared_ptr
*/
/* Macaroni::Build::BuilderArgumentsPtr
     ld    : namespace Macaroni { namespace Build { 
typedef boost::shared_ptr<Macaroni::Build::BuilderArguments >  BuilderArgumentsPtr;} } // End namespace 
 
     hd    : #include <Macaroni/Build/_.h>
 
     using : using Macaroni::Build::BuilderArgumentsPtr;
 
     light dependencies : 
                   Macaroni::Build::BuilderArguments

     HEAVY dependencies : 
                   boost::shared_ptr
*/
/* Macaroni::Build::Cpp
     ld    : // ~ <(I don't know how to generate definition for Macaroni::Build::Cpp.)
 
     hd    : #include <Macaroni/Build/_.h>
 
     using : // ~ <(I don't know how to generate a using statement for Macaroni::Build::Cpp.) 
 
     light dependencies : 

     HEAVY dependencies : 
*/
/* Macaroni::Build::GeneratorContextPtr
     ld    : namespace Macaroni { namespace Build { 
typedef boost::shared_ptr<Macaroni::Build::GeneratorContext >  GeneratorContextPtr;} } // End namespace 
 
     hd    : #include <Macaroni/Build/_.h>
 
     using : using Macaroni::Build::GeneratorContextPtr;
 
     light dependencies : 
                   Macaroni::Build::GeneratorContext

     HEAVY dependencies : 
                   boost::shared_ptr
*/

#include <boost/shared_ptr.hpp>
#include <Macaroni/Build/GeneratorContext.h>
namespace Macaroni { namespace Build { 
class GeneratorContext;
} } // End namespace 
#include <boost/shared_ptr.hpp>
#include <Macaroni/Build/BuilderArguments.h>
namespace Macaroni { namespace Build { 
class BuilderArguments;
} } // End namespace 
#include <boost/shared_ptr.hpp>
#include <Macaroni/Build/BuildContext.h>
namespace Macaroni { namespace Build { 
class BuildContext;
} } // End namespace 

namespace Macaroni { namespace Build { 

typedef boost::shared_ptr<Macaroni::Build::BuildContext > BuildContextPtr;
typedef boost::shared_ptr<Macaroni::Build::BuilderArguments > BuilderArgumentsPtr;
//     ~ Have no way to handle node Cpp with Member type Namespace.
typedef boost::shared_ptr<Macaroni::Build::GeneratorContext > GeneratorContextPtr;

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build___H