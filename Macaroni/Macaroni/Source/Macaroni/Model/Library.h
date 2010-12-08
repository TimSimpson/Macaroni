#ifndef MACARONI_MODEL_LIBRARY_H
#define MACARONI_MODEL_LIBRARY_H

#include "../ME.h"
#include "Context.h"
#include "FileNamePtr.h"
#include <Macaroni/Build/LibraryId.h>
#include "LibraryPtr.h"
#include "MemberPtr.h"
#include "NodePtr.h"
#include <Macaroni/IO/Path.h>
#include <vector>

BEGIN_NAMESPACE2(Macaroni, Model)

/** Nodes always belong to a library.  They coexist next to each other in the 
 * Model tree, but they all keep a reference to the library they were created
 * under. */
class Library
{ 
friend Context;
friend void intrusive_ptr_add_ref(Library * p);
friend void intrusive_ptr_release(Library * p);

public:			
	~Library();

	inline ContextPtr GetContext() const
	{
		return ContextPtr(context);
	}
	
	/** Adds the dependency if not already in this library's dependency list. */
	void AddDependency(const LibraryPtr & dependency);
	
	/** Return the root path where these files are installed to.
	 * The files might not be there if the library is what is being built!
	 */
	Macaroni::IO::PathPtr FindInstallPath() const;

	std::vector<LibraryPtr> GetDependencies() const;
	
	inline const std::string & GetGroup() const
	{
		return id.GetGroup();
	}	

	inline const Macaroni::Build::LibraryId & GetId() const
	{
		return id;	
	}

	inline const std::string & GetName() const
	{
		return id.GetName();
	}

	inline const std::string & GetVersion() const
	{
		return id.GetVersion();
	}

private:
	Library(Context * context, const std::string & name, const std::string & version);
	
	Library(Context * context, const Macaroni::Build::LibraryId & id);

	Context * context;
	std::vector<Library *> dependencies;
	int referenceCount;
	Macaroni::Build::LibraryId id;
};

END_NAMESPACE2

#endif
