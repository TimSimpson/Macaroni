/**
 * Copyright 2011 Tim Simpson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MACARONI_BUILD_MANIFEST_H
#define MACARONI_BUILD_MANIFEST_H

#include <boost/filesystem/operations.hpp>

namespace Macaroni { namespace Build {
	class Manifest;
	typedef boost::shared_ptr<Manifest> ManifestPtr;
} }


#include <Macaroni/Build/BuildContextPtr.h>
#include "Configuration.h"
#include <Macaroni/Environment/Console.h>
#include "../Environment/LuaEnvironment.h"
#include <Macaroni/Build/BuildContext.h>
#include <Macaroni/Build/LibraryId.h>
#include <Macaroni/Model/LibraryPtr.h>
#include "../ME.h"
#include "../IO/Path.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <boost/variant.hpp>
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
			 const std::string & properties,
			 ManifestPtr upperManifest,
			 const bool isCavatappi);

    ~Manifest();

	//TODO: This may not be a good idea.  Consider removing.
	bool AllowChildFailure() const;

	inline const bool ContainsCavatappi() const
	{
		return containsCavatappi;
	}

	inline const std::vector<MACARONI_VE_CONST std::string> GetBugs() const
	{
		return bugs;
	}

	inline const std::vector<MACARONI_VE_CONST std::string> GetChildren() const
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
	inline const std::vector<MACARONI_VE_CONST std::string> GetMSource() const
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

	void SaveAs(const boost::filesystem::path & filePath,  
				std::vector<std::pair<std::string, std::string> > & runList);

	/** Called when a bug in this Manifest's bug list is executed.
	 *  Argument libraryResult is null if the nothing was installed. */
	void SetBugResult(const std::string & bugName, 
					  Macaroni::Model::LibraryPtr libraryResult,
					  boost::optional<std::string> description);

	inline void SetChildren(std::vector<MACARONI_VE_CONST std::string> & value)
	{
		children = value;
	}
	
	inline void SetDescription(std::string & value)
	{
		description = value;
	}

	inline void SetMSource(std::vector<MACARONI_VE_CONST std::string> & value)
	{
		mSource = value;
	}

private:
	const bool allowCavatappi;
	bool allowChildFailure;
	std::vector<MACARONI_VE_CONST std::string> bugs;	
	std::vector<MACARONI_VE_CONST std::string> children;		
	std::vector<MACARONI_VE_CONST Configuration> configurations;
	bool containsCavatappi;
	std::string cppHeadersOutput;
	std::string cppOutput;
	std::string cppSourceOutput;
	std::vector<LibraryId> dependencies;
	std::string description;
	std::string fOutput;
	//std::vector<std::string> generators;
	std::string group;
	LibraryId id;	
	Macaroni::Environment::LuaEnvironment luaEnv;
	boost::filesystem::path manifestFile;
	std::string mOutput;
	std::vector<MACARONI_VE_CONST std::string> mSource;
	std::string name;
	std::string properties;
	boost::filesystem::path rootDirectory;
	std::string version;
};


} }

#endif