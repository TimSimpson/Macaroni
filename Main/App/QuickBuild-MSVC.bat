SET LIBRARY_PATH=C:\Users\Tim\Tools\Local\Boost\boost_1_52_0\stage\lib

REM Can't figure out how to make this work without supplying the suffix.
SET BOOST_LIB_SUFFIX=-vc110-mt-gd-1_52
bjam -d+2 -j8 link=static threading=multi
bin\msvc-11\debug\link-static\threading-multi\macaroni.exe luaTests Source\test\lua
