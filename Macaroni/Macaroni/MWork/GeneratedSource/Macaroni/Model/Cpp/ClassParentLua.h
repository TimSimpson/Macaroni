#ifndef MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp_ClassParent_LUA_GLUE_H
#define MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp_ClassParent_LUA_GLUE_H
#include <string>
#include <Macaroni/Model/Cpp/ClassParent.h>

#include <Macaroni/Model/Cpp/_.h>


struct lua_State;

namespace Macaroni { namespace Model { namespace Cpp { 
struct ClassParentLuaMetaData
{
    static bool IsType(lua_State * L, int index);
    static int OpenInLua(lua_State * L);
    static ClassParentPtr & GetInstance(lua_State * L, int index);
    static int Index(lua_State * L, ClassParentPtr & ptr, const std::string & index);
    static void PutInstanceOnStack(lua_State * L, const ClassParentPtr & ptr); 
};       
        } } } // End namespace 
#endif // end of MACARONI_COMPILE_GUARD_Macaroni_Model_Cpp_ClassParent_LUA_GLUE_H