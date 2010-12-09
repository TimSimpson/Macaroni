#ifndef MACARONI_MODEL_LIBRARY_CPP
#define MACARONI_MODEL_LIBRARY_CPP

#include "../ME.h"
#include "Context.h"
#include <boost/foreach.hpp>
#include "Library.h"

using Macaroni::Build::LibraryId;
using Macaroni::IO::Path;
using Macaroni::IO::PathPtr;

BEGIN_NAMESPACE2(Macaroni, Model)


Library::Library(Context * context, const std::string & name, const std::string & version)
:context(context),
 dependencies(),
 id(),
 referenceCount(0) 
{
	id.SetName(name);
	id.SetVersion(version);
	id.SetGroup("~..~");
}

Library::Library(Context * context, const LibraryId & id)
: context(context),
  dependencies(),
  id(id),
  referenceCount(0)
{
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
	boost::filesystem::path p(id.FindInstallPath());
	return PathPtr(new Path(p, p));
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
