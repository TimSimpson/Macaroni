#ifndef MACARONI_COMPILE_GUARD_Macaroni_IO_GeneratedFileWriter_H
#define MACARONI_COMPILE_GUARD_Macaroni_IO_GeneratedFileWriter_H

// This class was originally defined in Source/Macaroni/IO/GeneratedFileWriter.mcpp
// Forward declaration necessary if this depends on anything which also depend on this.
namespace Macaroni { namespace IO { 
class GeneratedFileWriter;
} } // End namespace 

#include <boost/shared_ptr.hpp>
#include <iostream>
#include <string>
#include <boost/filesystem/operations.hpp>
#include <sstream>
#include <Macaroni/IO/_.h>
namespace Macaroni { namespace IO { 
typedef boost::shared_ptr<std::ostream >  ostreamptr;} } // End namespace 

namespace Macaroni { namespace IO { 


/* Public Global Methods */

class GeneratedFileWriter
{
private: 	bool  closed;
private: 	bool  directToFile;
private: 	const boost::filesystem::path  filePath;
private: 	bool  fileWasWritten;
private: 	Macaroni::IO::ostreamptr  writer;
	public: GeneratedFileWriter(const boost::filesystem::path  filePath);
	public: ~GeneratedFileWriter();
public: 	bool  Close();
public: 	const boost::filesystem::path &  GetFilePath() const;
public: 	static bool  isFileDifferentThanBuffer(const boost::filesystem::path &  filePath, std::stringstream &  current);
public: 	static bool  overwriteFileWithBufferIfDifferent(const boost::filesystem::path &  filePath, std::stringstream &  current);
public: 	void  Write(const std::string &  text);
public: 	void  WriteLine(const std::string &  text);
}; // End of class GeneratedFileWriter

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_IO_GeneratedFileWriter_H