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
#ifndef MACARONI_MODEL_ATTRIBUTETABLELUA_H
#define MACARONI_MODEL_ATTRIBUTETABLELUA_H

#include "AnnotationTable.h"
#include "AnnotationValue.h"
#include "NodePtr.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct AnnotationTableLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static bool IsType(lua_State * L, int index, const char * type);
	static int OpenInLua(lua_State * L);
	static AnnotationTablePtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const AnnotationTablePtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const AnnotationTablePtr & ptr);
};


END_NAMESPACE2

#endif
