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
#ifndef MACARONI_GENERATOR_CPP_CPPSOURCEGENERATOR2_H
#define MACARONI_GENERATOR_CPP_CPPSOURCEGENERATOR2_H

#include "../../ME.h"
#include "../../Model/MemberVisitor.h"
#include <fstream>
#include <boost/filesystem/operations.hpp>

BEGIN_NAMESPACE(Macaroni, Generator, Cpp)

class CppSourceGenerator2
{
public:
	CppSourceGenerator2(ContextPtr context, const boost::filesystem::path & path);
	
	~CppSourceGenerator2();
	
private:

	boost::filesystem::path rootPath;
};

END_NAMESPACE

#endif

