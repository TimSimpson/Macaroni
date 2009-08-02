#ifndef MACARONI_MODEL_FILENAMELUA_H
#define MACARONI_MODEL_FILENAMELUA_H

#include "FileNamePtr.h"

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, Model)

struct FileNameLuaMetaData
{
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static FileNamePtr & GetInstance(lua_State * L, int index);
	static void PutInstanceOnStack(lua_State * L, const FileNamePtr & ptr);
};


END_NAMESPACE2

#endif
