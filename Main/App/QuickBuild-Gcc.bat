REM cd ..\Dependencies\Lua
REM macaroni install

macaroni generatorPath ..\Generators generate
SET LIBRARY_PATH=C:\Users\Tim\Tools\Local\Boost\boost_1_52_0\stage\lib

REM Can't figure out how to make this work without supplying the suffix.
SET BOOST_LIB_SUFFIX=-mgw47-mt-1_52
bjam -d+2 --toolset=gcc cxxflags=-std=gnu++11 link=static threading=multi
bin\gcc-mingw-4.7.2\debug\link-static\threading-multi\macaroni.exe luaTests Source\test\lua
