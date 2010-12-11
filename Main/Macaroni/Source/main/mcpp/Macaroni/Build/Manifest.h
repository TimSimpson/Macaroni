#ifndef MACARONI_BUILD_MANIFEST_H
#define MACARONI_BUILD_MANIFEST_H

#include <boost/filesystem/operations.hpp>

namespace Macaroni { namespace Build {
	class Manifest;
	typedef boost::shared_ptr<Manifest> ManifestPtr;
} }


#include <Macaroni/Build/_.h>
#include "Configuration.h"
#include <Macaroni/Environment/Console.h>
#include "../Environment/LuaEnvironment.h"
#include <Macaroni/Build/BuildContext.h>
#include <Macaroni/Build/LibraryId.h>
#include "../ME.h"
#include "../IO/Path.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

namespace Macaroni { namespace Build {

class Manifest;
typedef boost::shared_ptr<Manifest> ManifestPtr;


class Manifest
{	
public:
	typedef std::pair<std::string, std::string> RunEntry;

	struct RunResult
	{
		bool Success;
		 std::vector<RunEntry> RunList;
	};

	typedef boost::shared_ptr<RunResult> RunResultPtr;

	Manifest();

	Manifest(LibraryId id, std::vector<LibraryId> deps);

	Manifest(const boost::filesystem::path & manifestFile, 
			 const std::string & properties);

    ~Manifest();

	inline const std::vector<const std::string> GetChildren() const
	{
		return children;
	}

	inline std::string GetDescription() const
	{
		return description;
	}

	inline const std::string & GetCppOutput() const
	{
		return cppOutput;
	}

	inline const std::string & GetCppSourceOutput() const
	{
		return cppSourceOutput;
	}

	const Configuration * GetConfiguration(const std::string & configName) const;

	inline const LibraryId & GetId() const
	{
		return id;
	}

	inline const std::string GetCppHeadersOutput() const
	{
		return cppHeadersOutput;
	}

	inline const std::vector<LibraryId> & GetDependencies() 
	{
		return dependencies;
	}

	inline const std::string GetFinalOutput() const
	{
		return fOutput;
	}

	inline const std::string & GetGroup() const
	{
		return group;
	}

	inline const boost::filesystem::path & GetManifestFile() const
	{
		return manifestFile;
	}

	/** The Output directory.  This is stored as an absolute path, which I'm no
	 * longer sure I agree wtih. */
	inline const std::string GetMOutput() const
	{
		return mOutput;
	}

	inline const std::string & GetName() const
	{
		return name;
	}

	std::string GetProperties();

	inline const boost::filesystem::path & GetRootDirectory() const
	{
		return rootDirectory;
	}

	/** The list of Source directories.  Each one is an absolute path. */
	inline const std::vector<const std::string> GetMSource() const
	{
		return mSource;
	}

	inline const std::string & GetVersion() const
	{
		return version;
	}

	//bool RunTarget(const Macaroni::Environment::Console & console, 
	//			   GeneratorContextPtr generatorContext, const std::string & name);

	RunResultPtr RunTarget(const Macaroni::Environment::Console & console,
		           BuildContextPtr iContext, 
				   const std::string & manifestMethodName, 
				   const std::string & generatorMethodName);

	void SaveAs(boost::filesystem::path & filePath,  
				std::vector<std::pair<std::string, std::string>> & runList);

	inline void SetChildren(std::vector<const std::string> & value)
	{
		children = value;
	}
	
	inline void SetDescription(std::string & value)
	{
		description = value;
	}

	inline void SetMSource(std::vector<const std::string> & value)
	{
		mSource = value;
	}

private:
	std::vector<const std::string> children;
	std::vector<const Configuration> configurations;
	std::string cppOutput;
	std::string cppSourceOutput;
	std::vector<LibraryId> dependencies;
	std::string description;
	std::string fOutput;
	//std::vector<std::string> generators;
	std::string group;
	LibraryId id;
	std::string cppHeadersOutput;
	Macaroni::Environment::LuaEnvironment luaEnv;
	boost::filesystem::path manifestFile;
	std::string mOutput;
	std::vector<const std::string> mSource;
	std::string name;
	std::string properties;
	boost::filesystem::path rootDirectory;
	std::string version;
};


} }

#endif
