REM cd ..\Dependencies\Lua
REM macaroni install

macaroni generatorPath ..\Generators generate
bjam -d+2 link=static threading=multi
