#ifndef MACARONI_COMPILE_GUARD_Macaroni_IO_GeneratedFileWriter_CPP
#define MACARONI_COMPILE_GUARD_Macaroni_IO_GeneratedFileWriter_CPP

#include "GeneratedFileWriter.h"
#include <boost/filesystem/convenience.hpp>
#include <Macaroni/Exception.h>
#include <Macaroni/IO/GeneratedFileWriter.h>
#include <iostream>
#include <fstream>
#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/shared_ptr.hpp>
#include <string>
#include <sstream>

using boost::filesystem::create_directories;
using Macaroni::Exception;
using Macaroni::IO::GeneratedFileWriter;
using std::ofstream;
using std::ifstream;
using std::ostream;
using boost::filesystem::path;
using boost::shared_ptr;
using std::string;
using std::stringstream;

namespace Macaroni { namespace IO { 

/* Adopted Global Functions */
namespace {
} // end anonymous namespace
/* End globals. */

GeneratedFileWriter::GeneratedFileWriter(const path  filePath)
: closed(false), filePath(filePath), fileWasWritten(false), writer()
{
	
        if (boost::filesystem::exists(filePath)) 
        {
            if (!boost::filesystem::is_regular(filePath))
            {
                stringstream msg;
                msg << "Cannot create GeneratedFileWriter for " << filePath.string()
                    << ": the file exists and it is non-regular.";
                throw Macaroni::Exception(msg.str().c_str());
            }
            writer = ostreamptr(new stringstream());
            directToFile = false;
        }
        else
        {
            writer = ostreamptr(new ofstream(filePath.native_file_string().c_str(), ofstream::trunc));
            directToFile = true;
        }
    
}
GeneratedFileWriter::~GeneratedFileWriter()
{
	
        Close();
    
}
bool  GeneratedFileWriter::Close()
{
	
        if (!closed) 
        {
            if (directToFile) 
            {
                (dynamic_cast<ofstream *>(writer.get()))->close();
                fileWasWritten = true;
            }
            else
            {
                stringstream & ss = dynamic_cast<stringstream &>(
                    *(dynamic_cast<stringstream *>(writer.get()))
                    );
                fileWasWritten = overwriteFileWithBufferIfDifferent(filePath, ss);
            }
            closed = true;
        }
        return fileWasWritten;
    
}
const path &  GeneratedFileWriter::GetFilePath() const
{
	
        return filePath;
    
}
bool  GeneratedFileWriter::isFileDifferentThanBuffer(const path &  filePath, stringstream &  current)
{
	
        char buf1[256];
        char buf2[256];
        
        ifstream existing(filePath.native_file_string().c_str());
        current.seekg(std::ios_base::beg);

        bool eof = false;
        while(!eof)
        {
            existing.read(buf1, 256);
            current.read(buf2, 256);
            if (existing.gcount() != current.gcount())
            {
                return true;
            }
            else if (existing.gcount() != 256)
            {
                eof = true;
            }
            if (strncmp(buf1, buf2, existing.gcount()) != 0)
            {
                return true;
            }
        }
        return false;
    
}
bool  GeneratedFileWriter::overwriteFileWithBufferIfDifferent(const path &  filePath, stringstream &  current)
{
	
        if (!isFileDifferentThanBuffer(filePath, current))
        {
            return false;
        }
        current.seekg(std::ios_base::beg);
        ofstream output(filePath.native_file_string().c_str(), ofstream::trunc);
        output << current.str() ;
        output.close();
        return true;
    
}
void  GeneratedFileWriter::Write(const string &  text)
{
	
        (*writer) << text;
    
}
void  GeneratedFileWriter::WriteLine(const string &  text)
{
	
        (*writer) << text << std::endl;
    
}

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_IO_GeneratedFileWriter_CPP