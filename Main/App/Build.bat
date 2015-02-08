SET LIBRARY_PATH=C:\Users\Tim\Tools\Local\Boost\boost_1_52_0\stage\lib

REM Can't figure out how to make this work without supplying the suffix.
REM SET BOOST_LIB_SUFFIX=-mgw47-mt-1_52
REM bjam -d+2 -j8 --toolset=gcc cxxflags=-std=gnu++11 link=static threading=multi
REM bin\gcc-mingw-4.7.2\debug\link-static\threading-multi\macaroni.exe luaTests Source\test\lua

macaroni --messagesPath=Source\main\resources\Messages.txt --libraryRepoPath=..\Libraries --generatorPath=..\Generators -b  --showPaths -o "link=static --toolset=msvc-12.0" --printTimes
