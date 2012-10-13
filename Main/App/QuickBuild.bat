REM cd ..\Dependencies\Lua
REM macaroni install

macaroni generatorPath ..\Generators generate

SET BOOST_LIB_SUFFIX=-vc110-mt-gd-1_51
bjam -d+2 --toolset=msvc link=static threading=multi exe

