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
#ifndef MACARONI_MODEL_CPP_TYPEINFOLUA_CPP
#define MACARONI_MODEL_CPP_TYPEINFOLUA_CPP

#include "../NodeLua.h"
#include "TypeInfoLua.h"
#include <Macaroni/Lua.h>

struct lua_State;

BEGIN_NAMESPACE(Macaroni, Model, Cpp)

int TypeInfoLuaMetaData::Index(lua_State * L, const TypeInfo & var,
								const std::string & index)
{
	if (index == "Const")
	{
		lua_pushboolean(L, var.IsConst ? 1 : 0);
		return 1;
	}
	else if (index == "ConstPointer")
	{
		lua_pushboolean(L, var.IsConstPointer ? 1 : 0);
		return 1;
	}
	else if (index == "Pointer")
	{
		lua_pushboolean(L, var.IsPointer ? 1 : 0);
		return 1;
	}
	else if (index == "Reference")
	{
		lua_pushboolean(L, var.IsReference ? 1 : 0);
		return 1;
	}
	else if (index == "Node")
	{
		Model::NodeLuaMetaData::PutInstanceOnStack(L, var.Node);
		return 1;
	}
	return 0;
}


END_NAMESPACE

#endif
