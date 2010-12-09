#ifndef MACARONI_COMPILE_GUARD_Macaroni_Build_ManifestOrganizer_CPP
#define MACARONI_COMPILE_GUARD_Macaroni_Build_ManifestOrganizer_CPP

#include "ManifestOrganizer.h"
#include <boost/foreach.hpp>
#include <Macaroni/Exception.h>
#include <Macaroni/Model/Library.h>
#include <Macaroni/Build/LibraryId.h>
#include <Macaroni/Model/LibraryPtr.h>
#include <Macaroni/Build/Manifest.h>
#include <Macaroni/Build/Manifest.h>
#include <boost/shared_ptr.hpp>
#include <string>
#include <sstream>
#include <vector>

using Macaroni::Exception;
using Macaroni::Model::Library;
using Macaroni::Build::LibraryId;
using Macaroni::Model::LibraryPtr;
using Macaroni::Build::Manifest;
using Macaroni::Build::ManifestPtr;
using boost::shared_ptr;
using std::string;
using std::stringstream;
using std::vector;

namespace Macaroni { namespace Build { 

/* Adopted Global Functions */
namespace {
} // end anonymous namespace
/* End globals. */

ManifestOrganizer::ManifestOrganizer(std::vector<Macaroni::Build::ManifestPtr > &  manifests)
: list(manifests), listHistory()
{
	 
        listHistory.reserve(list.size());
        for (unsigned int i = 0; i < list.size(); i ++)
        {
            const LibraryId & libI = list[i]->GetId();
            listHistory.push_back(vector<LibraryId>());
            //listHistory[i].push_back(libI);
        }
    
}
void  ManifestOrganizer::clearHistory()
{
	
        // I used to think the history of every move should always be
        // preserved.  Then I noticed that really, I only care about the history
        // within a single move, just to ensure things aren't infinitely 
        // swapping.
        BOOST_FOREACH(vector<LibraryId> & history, listHistory)
        {
            history.clear();
        }
    
}
bool  ManifestOrganizer::dependencyListContains(const std::vector<Macaroni::Build::LibraryId > &  dependencies, const LibraryId &  id)
{
	
        BOOST_FOREACH(const LibraryId & dep, dependencies)
        {
            if (dep == id)
            {
                return true;
            }
        }
        return false;
    
}
std::vector<Macaroni::Build::ManifestPtr >  ManifestOrganizer::GetList()
{
	
        return list;
    
}
bool  ManifestOrganizer::positionPreviouslyContainedId(const int  position, const LibraryId &  id)
{
	
        vector<LibraryId> & history = listHistory.at(position);
        BOOST_FOREACH(const LibraryId & lib, history)
        {
            if (lib == id)
            {
                return true;
            }
        }    
        return false;
    
}
void  ManifestOrganizer::set(int  position, ManifestPtr  manifest)
{
	
        if (position >= list.size() || position < 0) 
        {
            stringstream ss;
            ss << "Set for position " << position << " is not valid.";
            throw Exception(ss.str().c_str());
        }
        const LibraryId & libId = manifest->GetId();        
        list[position] = manifest;
        listHistory[position].push_back(manifest->GetId());    
    
}
void  ManifestOrganizer::Sort()
{
	        
        for (unsigned int i = 0; i < list.size(); i ++)
        {
            clearHistory();
            ManifestPtr iM = list[i];            
            for (unsigned int j = i + 1; j < list.size(); j ++)
            {
                if (dependencyListContains(iM->GetDependencies(), 
                                           list[j]->GetId()))
                {
                    if (swap(i, j))
                    {
                        i --; // redo I with its new value
                        break;
                    } // If swap fails, just proceed.
                }        
            }
        }
    
}
bool  ManifestOrganizer::swap(int  a, int  b)
{
	
        if (a >= b)
        {
            std::stringstream ss;
            ss    << "A can't be more than B. A = " << a 
                << " and B = " << b << ".";
            throw Exception(ss.str().c_str());
        }

        const LibraryId & libB = list.at(b)->GetId();
        // Do not swap if we have done this before...
        if (positionPreviouslyContainedId(a, libB))
        {
            return false;
        }
        ManifestPtr mA = list.at(a);
        ManifestPtr mB = list.at(b);
        set(a, mB);
        set(b, mA);
        return true;
    
}

} } // End namespace 

#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Build_ManifestOrganizer_CPP