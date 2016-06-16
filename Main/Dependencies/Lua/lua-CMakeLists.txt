PROJECT(Lua CXX)
cmake_minimum_required(VERSION 2.8)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin)

SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})

include_directories(src)

set_source_files_properties(src/lapi.c
    src/lauxlib.c
    src/lbaselib.c
    src/lbitlib.c
    src/lcode.c
    src/lcorolib.c
    src/lctype.c
    src/ldblib.c
    src/ldebug.c
    src/ldo.c
    src/ldump.c
    src/lfunc.c
    src/lgc.c
    src/linit.c
    src/liolib.c
    src/llex.c
    src/lmathlib.c
    src/lmem.c
    src/loadlib.c
    src/lobject.c
    src/lopcodes.c
    src/loslib.c
    src/lparser.c
    src/lstate.c
    src/lstring.c
    src/lstrlib.c
    src/ltable.c
    src/ltablib.c
    src/ltm.c
    src/lua.c
    src/luac.c
    src/lundump.c
    src/lvm.c
    src/lzio.c
    src/lua.c
    PROPERTIES LANGUAGE CXX)

ADD_LIBRARY(lua STATIC
    src/lapi.c
    src/lauxlib.c
    src/lbaselib.c
    src/lbitlib.c
    src/lcode.c
    src/lcorolib.c
    src/lctype.c
    src/ldblib.c
    src/ldebug.c
    src/ldo.c
    src/ldump.c
    src/lfunc.c
    src/lgc.c
    src/linit.c
    src/liolib.c
    src/llex.c
    src/lmathlib.c
    src/lmem.c
    src/loadlib.c
    src/lobject.c
    src/lopcodes.c
    src/loslib.c
    src/lparser.c
    src/lstate.c
    src/lstring.c
    src/lstrlib.c
    src/ltable.c
    src/ltablib.c
    src/ltm.c
    src/lua.c
    src/luac.c
    src/lundump.c
    src/lvm.c
    src/lzio.c
)

ADD_EXECUTABLE(lua-repl src/lua.c)
TARGET_LINK_LIBRARIES(lua-repl lua)
ADD_EXECUTABLE(luac src/luac.c)
TARGET_LINK_LIBRARIES(luac lua)

IF (WIN32 AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    TARGET_LINK_LIBRARIES(lua-repl stdc++)
    TARGET_LINK_LIBRARIES(luac stdc++)
ENDIF()
