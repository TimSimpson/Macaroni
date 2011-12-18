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
#ifndef MACARONI_MODEL_LIBRARY_H
#define MACARONI_MODEL_LIBRARY_H

#include "../ME.h"
#include <Macaroni/Model/Context.h>
#include <Macaroni/Model/ContextPtr.h>
#include "FileNamePtr.h"
#include <boost/optional.hpp>
#include <Macaroni/Build/LibraryId.h>
#include "LibraryPtr.h"
#include "MemberPtr.h"
#include "NodePtr.h"
#include <Macaroni/IO/Path.h>
#include <boost/filesystem/path.hpp>
#include <vector>

BEGIN_NAMESPACE2(Macaroni, Model)

/** Nodes always belong to a library.  They coexist next to each other in the
 * Model tree, but they all keep a reference to the library they were created
 * under. */
class Library
{
friend class Context;
friend void intrusive_ptr_add_ref(Library * p);
friend void intrusive_ptr_release(Library * p);

public:
	~Library();

	virtual inline ContextPtr GetContext() const
	{
		return ContextPtr(context);
	}

	/** Adds the dependency if not already in this library's dependency list. */
	virtual void AddDependency(const LibraryPtr & dependency);

	/** Return the root path where these files are installed to.
	 * The files might not be there if the library is what is being built!
	 */
	virtual Macaroni::IO::PathPtr FindInstallPath() const;

	virtual std::vector<LibraryPtr> GetDependencies() const;

	virtual inline const std::string & GetGroup() const
	{
		return id.GetGroup();
	}

	/** True if the artifacts are located in the repo, wherever that may be. */
	virtual inline bool IsInstalled() const
	{
		return !!FindInstallPath();
	}

	virtual inline std::string GetCId() const
	{
		return id.GetCId();
	}

	virtual inline const Macaroni::Build::LibraryId & GetId() const
	{
		return id;
	}

	virtual inline const std::string & GetName() const
	{
		return id.GetName();
	}

	virtual inline const std::string & GetVersion() const
	{
		return id.GetVersion();
	}

	/** Called if Library is installed. */
	virtual void SetInstallPath(boost::optional<boost::filesystem::path>
	                            installPath);

private:
	//TODO: Get rid of this constructor
	Library(Context * context, const std::string & name, const std::string & version);

	Library(Context * context, const Macaroni::Build::LibraryId & id,
			const boost::optional<boost::filesystem::path> & installPath);

	Context * context;
	std::vector<Library *> dependencies;
	int referenceCount;
	Macaroni::Build::LibraryId id;
	boost::optional<boost::filesystem::path> installPath;
};

END_NAMESPACE2

#endif
