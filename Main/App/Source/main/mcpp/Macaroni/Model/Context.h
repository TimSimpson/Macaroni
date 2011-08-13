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
#ifndef MACARONI_MODEL_CONTEXT_H
#define MACARONI_MODEL_CONTEXT_H

#include "../ME.h"
#include <boost/shared_ptr.hpp>
#include <boost/intrusive_ptr.hpp>
#include "ContextPtr.h"
#include <boost/optional.hpp>
#include <Macaroni/Build/LibraryId.h>
#include "LibraryPtr.h"
#include "NodePtr.h"
#include <boost/filesystem/path.hpp>

BEGIN_NAMESPACE2(Macaroni, Model)

/** This object holds a tree of Nodes.  In addition to providing the root Node,
 * it also stores a count of Node references.  Deleting this deletes all Nodes.
 */
class Context
{
friend void intrusive_ptr_add_ref(Context * p);
friend void intrusive_ptr_release(Context * p);
public:
	Context(const std::string & rootName);
	
	~Context();	

	LibraryPtr FindLibrary(const Macaroni::Build::LibraryId & id);

	LibraryPtr FindLibrary(const std::string & group, const std::string & name, const std::string & version);

	LibraryPtr FindOrCreateLibrary(const std::string & group, const std::string & name, const std::string & version);

	LibraryPtr FindOrCreateLibrary(const boost::optional<boost::filesystem::path> & installPath,
								   const Macaroni::Build::LibraryId & id);
	
	int GetReferenceCount() const;

	/** The root Node could be considered the global namespace. */
	NodePtr GetRoot();

	LibraryPtr GetRootLibrary();

private:

	bool hasBeenDeleted;

	std::vector<Library *> libraries;

	//ScopeMember * findMember(const Node * scope, const std::string & complexName);

	void onAddReference();
	
	void onReleaseReference();

	int referenceCount;
	
	Node * root;
	
};

inline void intrusive_ptr_add_ref(Context * p)
{
	p->onAddReference();
}

inline void intrusive_ptr_release(Context * p)
{
	p->onReleaseReference();
}



END_NAMESPACE2

#endif