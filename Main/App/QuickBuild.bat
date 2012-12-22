REM cd ..\Dependencies\Lua
REM macaroni install

macaroni generatorPath ..\Generators generate

SET BOOST_LIB_SUFFIX=-vc110-mt-gd-1_52
bjam -d+2 -j8 --toolset=msvc link=static threading=multi exe
bin\msvc-11.0\debug\link-static\threading-multi\macaroni.exe luaTests Source\test\lua
