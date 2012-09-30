REM cd ..\Dependencies\Lua
REM macaroni install

macaroni generatorPath ..\Generators generate

REM Can't figure out how to make this work without supplying the suffix.
SET BOOST_LIB_SUFFIX=-mgw47-mt-1_51
bjam -d+2 --toolset=gcc cxxflags=-std=gnu++11 link=static threading=multi
bjam -d+2 --toolset=gcc ^
     cxxflags=-std=gnu++11 link=static ^
     linkflags=-static-libgcc ^
     linkflags=-static-libstdc++ ^
     threading=multi release

REM SET BOOST_LIB_SUFFIX=-vc110-mt-d-1_51?
SET BOOST_LIB_SUFFIX=-vc110-mt-gd-1_51
bjam -d+2 --toolset=msvc link=static threading=multi
bjam -d+2 --toolset=msvc link=static threading=multi release

