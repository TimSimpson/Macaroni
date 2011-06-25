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
#ifndef FILE_MACARONI_CONTAINERS_H
#define FILE_MACARONI_CONTAINERS_H

#include <Macaroni/ME.h>
#include <boost/foreach.hpp>
#include <vector>

/** This file works around some insanity in the code base.  I'm still not 100%
 * sure if the ability to have a vector of constant elements is coming in C++0x
 * or isn't, or if the current (widespread) usage of this in Macaroni has 
 * or hasn't introduced subtle bugs due to VC++'s tolerance of it.
 * I'd like to keep the current usages in in case this turns out to be doable
 * somehow even if it requires some major refactoring (since the existing code
 * assumed it was possible, simply changing it would not document this 
 * assumption anywhere and be dangerous). */

namespace Macaroni { namespace Containers {

typedef std::vector<MACARONI_VE_CONST std::string> VectorOfConstString;

#ifdef MACARONI_VE_CONST_IS_ALLOWED
	/** Converts a vector of some element to a vector of that element
	 * but constant. */
	template<typename T> void AssignVectorToConstElementVector(
		const std::vector<T> & src, std::vector<MACARONI_VE_CONST T> & dst) 
	{
		dst = std::vector<MACARONI_VE_CONST T>();
		BOOST_FOREACH(const T & elem, src)
		{
			dst.push_back(elem);
		}	
	}
#else
	template<typename T> inline void AssignVectorToConstElementVector(
		const std::vector<T> & src, std::vector<MACARONI_VE_CONST T> & dst) 
	{
		dst = src;
	}
	
#endif

} } // end ns

#endif

