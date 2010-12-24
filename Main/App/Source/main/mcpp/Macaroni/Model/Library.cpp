#ifndef MACARONI_MODEL_LIBRARY_CPP
#define MACARONI_MODEL_LIBRARY_CPP

#include "../ME.h"
#include "Context.h"
#include <boost/foreach.hpp>
#include "Library.h"
#include <Macaroni/Exception.h>

using Macaroni::Build::LibraryId;
using Macaroni::IO::Path;
using Macaroni::IO::PathPtr;

BEGIN_NAMESPACE2(Macaroni, Model)

//TODO: Get rid of this constructor
Library::Library(Context * context, const std::string & name, const std::string & version)
:context(context),
 dependencies(),
 id(),
 installPath(),
 referenceCount(0) 
{
	id.SetName(name);
	id.SetVersion(version);
	id.SetGroup("~..~");
}

Library::Library(Context * context, const LibraryId & id, 
				 const boost::optional<boost::filesystem::path> & installPath)
: context(context),
  dependencies(),
  id(id),
  installPath(installPath),
  referenceCount(0)
{
	if (!!installPath && installPath.get() == "")
	{
		throw Macaroni::Exception("Empty path is not valid for install path!");
	}
}

Library::~Library()
{	
}

void Library::AddDependency(const LibraryPtr & dependency)
{
	BOOST_FOREACH(Library * dep, dependencies)
	{
		if (dep->GetId() == dependency->GetId())
		{
			return; // Already included, so skip it.
		}
	}
	dependencies.push_back(dependency.get());
}

PathPtr Library::FindInstallPath() const
{
	if (!installPath)
	{
		PathPtr p;
		return p;
	}
	else 
	{		
		return PathPtr(new Path(installPath.get()));
	}
}

std::vector<LibraryPtr> Library::GetDependencies() const
{
	std::vector<LibraryPtr> rtnList;
	BOOST_FOREACH(Library * d, this->dependencies)
	{
		rtnList.push_back(LibraryPtr(d));
	}	
	return rtnList;
}

void intrusive_ptr_add_ref(Library * p)
{
	intrusive_ptr_add_ref(p->context);
}

void intrusive_ptr_release(Library * p)
{
	intrusive_ptr_release(p->context);	
}

END_NAMESPACE2

#endif
