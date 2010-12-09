#ifndef MACARONI_COMPILE_GUARD_Macaroni_IO___H
#define MACARONI_COMPILE_GUARD_Macaroni_IO___H
/* ~ Debug Output of Dependency Graph ~ */
/* Macaroni::IO::ostreamptr
     ld    : namespace Macaroni { namespace IO { 
typedef boost::shared_ptr<std::ostream >  ostreamptr;} } // End namespace 
 
     hd    : #include <Macaroni/IO/_.h>
 
     using : using Macaroni::IO::ostreamptr;
 
     light dependencies : 

     HEAVY dependencies : 
                   boost::shared_ptr
                   std::ostream
*/
/* Macaroni::IO::GeneratedFileWriterPtr
     ld    : namespace Macaroni { namespace IO { 
typedef boost::shared_ptr<Macaroni::IO::GeneratedFileWriter >  GeneratedFileWriterPtr;} } // End namespace 
 
     hd    : #include <Macaroni/IO/_.h>
 
     using : using Macaroni::IO::GeneratedFileWriterPtr;
 
     light dependencies : 
                   Macaroni::IO::GeneratedFileWriter

     HEAVY dependencies : 
                   boost::shared_ptr
*/

#include <boost/shared_ptr.hpp>
namespace Macaroni { namespace IO { 
class GeneratedFileWriter;
} } // End namespace 
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <Macaroni/IO/GeneratedFileWriter.h>

namespace Macaroni { namespace IO { 

typedef boost::shared_ptr<std::ostream > ostreamptr;
typedef boost::shared_ptr<Macaroni::IO::GeneratedFileWriter > GeneratedFileWriterPtr;

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_IO___H