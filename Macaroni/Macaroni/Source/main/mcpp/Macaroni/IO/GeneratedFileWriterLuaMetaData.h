#ifndef MACARONI_IO_GENERATEDFILEWRITERLUAMETADATA_H
#define MACARONI_IO_GENERATEDFILEWRITERLUAMETADATA_H

#include <Macaroni/ME.h>
#include <Macaroni/IO/GeneratedFileWriter.h>

struct lua_State;

BEGIN_NAMESPACE2(Macaroni, IO)

struct GeneratedFileWriterLuaMetaData
{	
	static bool IsType(lua_State * L, int index);
	static int OpenInLua(lua_State * L);
	static GeneratedFileWriterPtr & GetInstance(lua_State * L, int index);
	static int Index(lua_State * L, const GeneratedFileWriterPtr & ptr, const std::string & index);
	static void PutInstanceOnStack(lua_State * L, const GeneratedFileWriterPtr & ptr);
};


END_NAMESPACE2

#endif
