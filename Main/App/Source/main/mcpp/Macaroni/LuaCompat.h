#ifndef FILE_MACARONI_LUACOMPAT_H
#define FILE_MACARONI_LUACOMPAT_H

// Emulates old luaL_register function.
// Creates a new library table with all the given funcs,
// then stores a global variable of the same name.
#define MACARONI_LUA_REGISTER(L, name, funcs) \
    { luaL_newlib(L, funcs); \
      lua_setglobal(L, name); }

// Kind of emulates the old luaL_register function.
// Creates a new library table with all the given funcs,
// then stores a global variable of the same name,
// but *also* leaves a copy on the stack, so return 1 will
// return the table, which is the new way to do things in
// Lua 5.2.
#define MACARONI_LUA_REGISTER_FOR_RETURN(L, name, funcs) \
    { luaL_newlib(L, funcs); \
      lua_pushvalue(L, -1); \
      lua_setglobal(L, name); }


#endif

