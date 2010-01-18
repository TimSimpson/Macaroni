#ifndef MACARONI_BUILD_MCOMPILEROPTIONS_H
#define MACARONI_BUILD_MCOMPILEROPTIONS_H

#include <boost/filesystem/operations.hpp>
#include "../IO/FileSet.h"
#include <string>
#include <vector>

namespace Macaroni { namespace Build {

/** Defines options for the Macaroni compiler.  */
class MCompilerOptions
{
public:
	MCompilerOptions(const std::vector<Macaroni::IO::FileSet> & input, 
					 const boost::filesystem::path & output,
					 const std::vector<const std::string> generators);

	~MCompilerOptions();
	
	inline const std::vector<const std::string> GetGenerators() const
	{
		return generators;
	}

	inline const std::vector<Macaroni::IO::FileSet> & GetInput() const
	{
		return input;
	}

	inline const boost::filesystem::path & GetOutput() const
	{
		return output;
	}


private:
	const std::vector<const std::string> generators;
	std::vector<Macaroni::IO::FileSet> input;
	boost::filesystem::path output;	
};


} }

#endif
